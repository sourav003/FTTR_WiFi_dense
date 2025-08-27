/*
 * source.cc (modified for sequential transmission)
 *
 *  Created on: 30 July 2025
 *      Author: mondals
 */

#include <string.h>
#include <math.h>
#include <omnetpp.h>
#include <numeric>
#include <algorithm>
#include <fstream>   // <-- add this
#include <iostream>  // (optional, for debugging with std::cout)

#include "sim_params.h"
#include "ethPacket_m.h"
#include "ping_m.h"
#include "trigger_bsr_m.h"

using namespace std;
using namespace omnetpp;

class Background_Device : public cSimpleModule
{
private:
    cQueue source_queue;
    double Load;
    double ArrivalRate;
    double pkt_interval;
    double wireless_datarate;
    double wap_dist;
    double buffer_size = 0;
    double ul_tx_time = 0;
    vector<double> dist_values;

    cMessage *generateEvent = nullptr;
    cMessage *sendEvent = nullptr;
    cMessage *sendEventTxBound = nullptr;

public:
    virtual ~Background_Device();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual ethPacket *generateNewPacket();
};

Define_Module(Background_Device);

Background_Device::~Background_Device()
{
    cancelAndDelete(generateEvent);
    cancelAndDelete(sendEvent);
    cancelAndDelete(sendEventTxBound);

    // Clean up queues
    while (!source_queue.isEmpty()) {
        delete source_queue.pop();
    }
}

void Background_Device::initialize()
{
    gate("in")->setDeliverImmediately(true);

    wireless_datarate = par("throughput").doubleValue();

    std::ifstream file;
    if(dist_values.empty()) {
        if(strcmp(this->getName(),"bkgs1") == 0) {
            file.open("ap_bkg1.csv");
        }
        else if(strcmp(this->getName(),"bkgs2") == 0) {
            file.open("ap_bkg2.csv");
        }
        else if(strcmp(this->getName(),"bkgs3") == 0) {
            file.open("ap_bkg3.csv");
        }
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

    cModule *targetModule = getParentModule()->getSubmodule("waps", getIndex());
    sendDirect(png, 0, 0, targetModule->gate("Src_in"));
    EV << getFullName() << " Sending ping to " << targetModule->getFullName() << " at = " << simTime() << endl;

    source_queue.setName("source_queue");

    Load = par("load").doubleValue();
    double R_o = par("dataRate").doubleValue();
    ArrivalRate = Load*R_o/(8*pkt_sz_avg);
    EV << getFullName() << " wireless_datarate = " << wireless_datarate << ", wap_dist = " << wap_dist;
    EV << ", Load = " << Load << ", ArrivalRate = " << ArrivalRate << endl;

    generateEvent = new cMessage("generateEvent");  // initializing here
    sendEvent = new cMessage("sendEvent");
    sendEventTxBound = new cMessage("sendEventTxBound");

    // schedule first packet generation
    scheduleAt(simTime(), generateEvent);           // scheduling packet generation for the first time
}

void Background_Device::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(),"generateEvent") == 0) {
        // generate a packet and put into queue
        ethPacket *pkt = generateNewPacket();
        source_queue.insert(pkt);
        buffer_size += pkt->getByteLength();

        // schedule next packet generation
        pkt_interval = exponential(1/ArrivalRate);
        scheduleAt(simTime() + pkt_interval, generateEvent);

        EV << getFullName() << " Current buffer size = " << buffer_size << ", next packet generation is scheduled at = " << simTime()+pkt_interval << endl;

        // send packet if the channel is free
        /*if (!sendEvent->isScheduled()) {        // if no transmission is happening
            scheduleAt(simTime(), sendEvent);     // schedule send event immediately
        }*/
    }
    else if(strcmp(msg->getName(),"sendEvent") == 0) {
        if(!source_queue.isEmpty()) {
            // dequeue next packet
            ethPacket *pkt = check_and_cast<ethPacket *>(source_queue.pop());
            buffer_size -= pkt->getByteLength();

            cModule *targetModule = getParentModule()->getSubmodule("waps", getIndex());
            // compute delays
            simtime_t propDelay = wap_dist / (3e8);
            simtime_t txDuration = pkt->getBitLength() / wireless_datarate;
            // send it
            sendDirect(pkt, propDelay, txDuration, targetModule->gate("Src_in"));
            //sendDirect(pkt, 0, 0, targetModule->gate("Src_in"));
            scheduleAt(simTime()+txDuration, sendEvent);
            EV << getFullName() << " Sent background packet at = " << simTime();
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

            cModule *targetModule = getParentModule()->getSubmodule("waps", getIndex());
            simtime_t propDelay = wap_dist / (3e8);
            simtime_t txDuration = pkt->getBitLength() / wireless_datarate;
            sendDirect(pkt, propDelay, txDuration, targetModule->gate("Src_in"));
            EV << getFullName() << " Sent background packet at = " << simTime();
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

        cModule *targetModule = getParentModule()->getSubmodule("waps", getIndex());
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

ethPacket *Background_Device::generateNewPacket()
{
    int pkt_size = intuniform(64,1542);
    ethPacket *pkt = new ethPacket("bkg_data");
    pkt->setByteLength(pkt_size);
    pkt->setGenerationTime(simTime());
    //EV << getFullName() << " New packet generated with size (bytes): " << avgPacketSize << endl;
    return pkt;
}
