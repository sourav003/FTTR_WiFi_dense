/*
 * source_ctr.cc
 *
 *  Created on: 8 Aug 2025
 *      Author: mondals
 */

#include <string.h>
#include <math.h>
#include <omnetpp.h>
#include <numeric>   // Required for std::iota
#include <algorithm> // Required for std::sort
#include <fstream>   // <-- add this
#include <iostream>  // (optional, for debugging with std::cout)

#include "sim_params.h"
#include "ethPacket_m.h"
#include "ping_m.h"
#include "trigger_bsr_m.h"

using namespace std;
using namespace omnetpp;

/*
 * Each source generates packets following some probability distribution.
 * The average inter-arrival time is derived using the maximum datarate of the
 * SourceApp and average packet size.
 * The generateEvent is a self-message which is scheduled back-to-back to create
 * a new packet. Immediately the packet is transmitted to the corresponding ONU.
 */

class Control_Device : public cSimpleModule
{
    private:
        cQueue source_queue;                        // Queue for holding packets to be sent to ONUs/SFUs
        double avgPacketSize;
        double ArrivalRate;
        double pkt_interval;                     // inter-packet generation interval
        double wireless_datarate;
        double wap_dist;
        double buffer_size = 0;
        double ul_tx_time = 0;
        vector<double> dist_values;

        cMessage *generateEvent = nullptr;
        cMessage *sendEvent = nullptr; // new event to know when transmission finishes
        cMessage *sendEventTxBound = nullptr;

        //simsignal_t arrivalSignal;               // to send signals for statistics collection

    public:
        virtual ~Control_Device();

    protected:
        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual ethPacket *generateNewPacket();
};

// The module class needs to be registered with OMNeT++
Define_Module(Control_Device);

Control_Device::~Control_Device()
{
    cancelAndDelete(generateEvent);
    cancelAndDelete(sendEvent);
    cancelAndDelete(sendEventTxBound);

    // Clean up queues
    while (!source_queue.isEmpty()) {
        delete source_queue.pop();
    }
}

void Control_Device::initialize()
{
    gate("in")->setDeliverImmediately(true);

    wireless_datarate = par("throughput").doubleValue();

    std::ifstream file("ap_ctrl.csv");
    if(dist_values.empty()) {
        double value;
        while(file >> value) {
            dist_values.push_back(value);
        }
    }
    int idx = getIndex();
    wap_dist = (idx < (int)dist_values.size()) ? dist_values[idx] : par("wap_distance").doubleValue();
    file.close();
    //wap_dist = par("wap_distance").doubleValue();
    EV << getFullName() << " wap_distance = " << wap_dist << endl;

    ping *png = new ping("ping");      // sending ping message at T = 0 for finding the RTT of all ONUs
    png->setSTA_id(getIndex());
    png->setSTA_band_pref("5GHz");      // this is hard choice!
    png->setInter_node_dist(wap_dist);

    cModule *targetModule = getParentModule()->getSubmodule("waps", (getIndex()*2+1));
    sendDirect(png, 0, 0, targetModule->gate("Src_in"));
    EV << getFullName() << " Sending ping to " << targetModule->getFullName() << " at = " << simTime() << endl;

    source_queue.setName("source_queue");

    avgPacketSize = par("meanPacketSize").doubleValue();                      // get the avg packet size from NED file
    ArrivalRate   = par("sampleRate").doubleValue();                          // get the HMD location sample rate from NED file (1/11e-3 per sec)

    // Initialize variables
    double mean = 1e-3*(1.0/ArrivalRate);                       // mean = 10 ms
    double std = 4e-3;                                          // sd = 4 ms
    pkt_interval = truncnormal(mean, std);                      // packet inter-arrival times are generated following gaussian distribution

    generateEvent = new cMessage("generateEvent");              // self-message is generated for next packet generation
    //emit(arrivalSignal,pkt_interval);
    sendEvent = new cMessage("sendEvent");                      // initializing here
    sendEventTxBound = new cMessage("sendEventTxBound");

    // schedule first packet generation
    scheduleAt(simTime(), generateEvent);
}

void Control_Device::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(),"generateEvent") == 0) {
        // generate a packet and put into queue
        ethPacket *pkt = generateNewPacket();
        source_queue.insert(pkt);
        buffer_size += pkt->getByteLength();

        // schedule next packet generation
        double mean = 1e-3*(1.0/ArrivalRate);                       // mean = 11 ms
        double std = 1e-3;                                          // sd = 1 ms
        pkt_interval = truncnormal(mean, std);                      // packet inter-arrival times are generated following gaussian distribution
        scheduleAt(simTime()+pkt_interval, generateEvent);      // scheduling the next packet generation
        EV << getFullName() << " Next packet generation is scheduled at = " << simTime()+pkt_interval << endl;

        // send packet if the channel is free
        /*if (!sendEvent->isScheduled()) {        // if no transmission is happening
            scheduleAt(simTime(), sendEvent);   // schedule send event immediately
        }*/
    }
    else if(strcmp(msg->getName(),"sendEvent") == 0) {
        if(!source_queue.isEmpty()) {
            // dequeue next packet
            ethPacket *pkt = check_and_cast<ethPacket *>(source_queue.pop());
            buffer_size -= pkt->getByteLength();

            cModule *targetModule = getParentModule()->getSubmodule("waps", (getIndex()*2+1));
            // compute delays
            simtime_t propDelay = wap_dist / (3e8);
            simtime_t txDuration = pkt->getBitLength() / wireless_datarate;
            // send it
            sendDirect(pkt, propDelay, txDuration, targetModule->gate("Src_in"));
            //sendDirect(pkt, 0, 0, targetModule->gate("Src_in"));
            scheduleAt(simTime()+txDuration, sendEvent);
            EV << getFullName() << " Sent Control packet at = " << simTime();
            EV << " and next packet will be sent at = " << simTime()+txDuration << endl;
        }
        else {
            EV << getFullName() << " Queue is empty now!" << endl;
            // no need to re-schedule sendEvent in this case.
        }
    }
    else if(strcmp(msg->getName(),"sendEventTxBound") == 0) {
        if(!source_queue.isEmpty()) {
            ethPacket *pkt = check_and_cast<ethPacket *>(source_queue.pop());
            buffer_size -= pkt->getByteLength();

            cModule *targetModule = getParentModule()->getSubmodule("waps", (getIndex()*2+1));
            simtime_t propDelay = wap_dist / (3e8);
            simtime_t txDuration = pkt->getBitLength() / wireless_datarate;
            sendDirect(pkt, propDelay, txDuration, targetModule->gate("Src_in"));
            EV << getFullName() << " Sent control packet at = " << simTime();
            EV << " and next packet may be sent at = " << simTime()+txDuration << endl;

            ul_tx_time -= txDuration.dbl();                       // transmission time of current packet is reduced
            if(ul_tx_time > 0) {
                if(!source_queue.isEmpty()) {                     // need to check if the remaining ul_tx_time is sufficient to transmit the next packet
                    ethPacket *front = (ethPacket *)source_queue.front();
                    double txLatency = front->getBitLength() / wireless_datarate;
                    if(ul_tx_time >= txLatency)
                        scheduleAt(simTime()+txDuration, sendEventTxBound);
                }
            }
        }
        else {
            EV << getFullName() << " Queue is empty now!" << endl;
            // no need to re-schedule sendEvent in this case.
        }
    }
    else if((strcmp(msg->getName(),"trigger_6GHz") == 0)||(strcmp(msg->getName(),"trigger_5GHz")) == 0) {
        trigger_bsr *trg = check_and_cast<trigger_bsr *>(msg);
        wireless_datarate = trg->getThroughput();
        ul_tx_time = trg->getTX_time();
        delete trg;

        trigger_bsr *bsr = new trigger_bsr("bsr");
        bsr->setByteLength(64);
        bsr->setSTA_band_pref("5GHz");                          // currently preferred band is 5 GHz
        bsr->setSTA_buffer_size(buffer_size);

        cModule *targetModule = getParentModule()->getSubmodule("waps", (getIndex()*2+1));
        // compute delays
        simtime_t propDelay = wap_dist / (3e8);
        simtime_t txDuration = bsr->getBitLength() / wireless_datarate;
        // send it
        sendDirect(bsr, propDelay, txDuration, targetModule->gate("Src_in"));
        EV << getFullName() << " Sent BSR to = " << targetModule->getFullName() << " at " << simTime() << endl;

        // schedule ul transmission after SIFS
        if((ul_tx_time > 0)&&(!sendEventTxBound->isScheduled())) {
            scheduleAt(simTime()+wifi_sifs_time, sendEventTxBound);
        }
        // ignoring ACK and other control messages intentionally as their time gaps are numerically considered
    }
}

ethPacket *Control_Device::generateNewPacket()
{
    ethPacket *pkt = new ethPacket("control_data");
    pkt->setByteLength(avgPacketSize);                              // generating packets of same size
    pkt->setGenerationTime(simTime());
    //EV << getFullName() << " New packet generated with size (bytes): " << avgPacketSize << endl;
    return pkt;
}




