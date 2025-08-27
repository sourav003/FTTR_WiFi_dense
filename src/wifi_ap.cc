/*
 * wifi_ap.cc
 *
 *  Created on: 8 Aug 2025
 *      Author: mondals
 */

#include <string.h>
#include <math.h>
#include <omnetpp.h>
#include <numeric>   // Required for std::iota
#include <algorithm> // Required for std::sort

#include "sim_params.h"
#include "ethPacket_m.h"
#include "ping_m.h"
#include "trigger_bsr_m.h"

using namespace std;
using namespace omnetpp;

class WiFi_AP : public cSimpleModule
{
    private:
        //cQueue wap_queue;
        vector<double> sta_dist_6G;
        vector<double> sta_bsr_6G;
        vector<double> sta_bw_6G;
        vector<double> sta_bphz_6G;
        vector<double> sta_datarate_6G;
        vector<double> sta_tx_time_6G;
        vector<string> sta_name_6G;
        vector<cModule*> sta_modules_6G;

        vector<double> sta_dist_5G;
        vector<double> sta_bsr_5G;
        vector<double> sta_bw_5G;
        vector<double> sta_bphz_5G;
        vector<double> sta_datarate_5G;
        vector<double> sta_tx_time_5G;
        vector<string> sta_name_5G;
        vector<cModule*> sta_modules_5G;

        cMessage *scheduleTx6GHz = nullptr;
        cMessage *scheduleTx5GHz = nullptr;

        simsignal_t latencySignalXr;
        simsignal_t latencySignalHmd;
        simsignal_t latencySignalCtr;
        simsignal_t latencySignalHpt;
        simsignal_t latencySignalBkg;

    public:
        virtual ~WiFi_AP();

    protected:
        double ber;
        long totalBitsReceived = 0;
        long totalPacketsReceived = 0;
        long corruptedPackets = 0;

        // The following redefined virtual function holds the algorithm.
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

Define_Module(WiFi_AP);

WiFi_AP::~WiFi_AP()
{
    cancelAndDelete(scheduleTx6GHz);
    cancelAndDelete(scheduleTx5GHz);
}

void WiFi_AP::initialize()
{
    gate("Src_in")->setDeliverImmediately(true);

    // registering the signals
    latencySignalXr = registerSignal("xr_latency");
    latencySignalHmd = registerSignal("hmd_latency");
    latencySignalCtr = registerSignal("ctrl_latency");
    latencySignalHpt = registerSignal("hptc_latency");
    latencySignalBkg = registerSignal("bkg_latency");

    // start data transmissions in 6 GHz and 5 GHz bands
    scheduleTx6GHz = new cMessage("scheduleTx6GHz");                // initializing here
    scheduleAt(simTime()+SimTime(200e-6), scheduleTx6GHz);          // DL:UL = 20:80
    scheduleTx5GHz = new cMessage("scheduleTx5GHz");
    scheduleAt(simTime()+SimTime(500e-6), scheduleTx5GHz);          // DL:UL = 50:50
}

void WiFi_AP::handleMessage(cMessage *msg)
{
    if(msg->isPacket() == true) {
        if(strcmp(msg->getName(),"bkg_data") == 0) {                // updating buffer size after receiving requests from ONUs
            ethPacket *pkt = check_and_cast<ethPacket *>(msg);

            pkt->setWapArrivalTime(pkt->getArrivalTime());

            double bkg_packet_latency = pkt->getArrivalTime().dbl() - pkt->getGenerationTime().dbl();
            EV << getFullName() << " background packet_latency: " << bkg_packet_latency << endl;
            emit(latencySignalBkg, bkg_packet_latency);

            delete pkt;
        }
        else if(strcmp(msg->getName(),"xr_data") == 0) {            // updating buffer size after receiving requests from ONUs
            ethPacket *pkt = check_and_cast<ethPacket *>(msg);

            pkt->setWapArrivalTime(pkt->getArrivalTime());

            double xr_packet_latency = pkt->getArrivalTime().dbl() - pkt->getGenerationTime().dbl();
            EV << getFullName() << " XR packet_latency: " << xr_packet_latency << endl;
            emit(latencySignalXr, xr_packet_latency);

            delete pkt;
        }
        else if(strcmp(msg->getName(),"hmd_data") == 0) {           // updating buffer size after receiving requests from ONUs
            ethPacket *pkt = check_and_cast<ethPacket *>(msg);

            pkt->setWapArrivalTime(pkt->getArrivalTime());

            double hmd_packet_latency = pkt->getArrivalTime().dbl() - pkt->getGenerationTime().dbl();
            EV << getFullName() << " HMD packet_latency: " << hmd_packet_latency << endl;
            emit(latencySignalHmd, hmd_packet_latency);

            delete pkt;
        }
        else if(strcmp(msg->getName(),"control_data") == 0) {       // updating buffer size after receiving requests from ONUs
            ethPacket *pkt = check_and_cast<ethPacket *>(msg);

            pkt->setWapArrivalTime(pkt->getArrivalTime());

            double ctrl_packet_latency = pkt->getArrivalTime().dbl() - pkt->getGenerationTime().dbl();
            EV << getFullName() << " Control packet_latency: " << ctrl_packet_latency << endl;
            emit(latencySignalCtr, ctrl_packet_latency);

            delete pkt;
        }
        else if(strcmp(msg->getName(),"haptic_data") == 0) {        // updating buffer size after receiving requests from ONUs
            ethPacket *pkt = check_and_cast<ethPacket *>(msg);

            pkt->setWapArrivalTime(pkt->getArrivalTime());

            double hptc_packet_latency = pkt->getArrivalTime().dbl() - pkt->getGenerationTime().dbl();
            EV << getFullName() << " Haptic packet_latency: " << hptc_packet_latency << endl;
            emit(latencySignalHpt, hptc_packet_latency);

            delete pkt;
        }
        else if(strcmp(msg->getName(),"bsr") == 0) {                            // updating buffer size after receiving requests from ONUs
            trigger_bsr *bsr = check_and_cast<trigger_bsr *>(msg);

            if(strcmp(bsr->getSTA_band_pref(),"6GHz") == 0) {
                auto it = std::find(sta_name_6G.begin(), sta_name_6G.end(), bsr->getSenderModule()->getName());
                if (it != sta_name_6G.end()) {
                    int index = std::distance(sta_name_6G.begin(), it);         // get index
                    sta_bsr_6G[index] = bsr->getSTA_buffer_size();
                    EV << getFullName() << " BSR for " << bsr->getSenderModule()->getFullName() << " updated : " << sta_bsr_6G[index] << endl;

                    if(sta_bsr_6G[index] == 0) {        // empty buffer is reported
                        sta_tx_time_6G[index] = 0;
                    }
                    else {
                        sta_tx_time_6G[index] = 500e-6;
                    }
                }
                else {
                    EV << getFullName() << " Source of BSR NOT found!!! " << endl;
                }
            }
            else if(strcmp(bsr->getSTA_band_pref(),"5GHz") == 0) {
                auto it = std::find(sta_name_5G.begin(), sta_name_5G.end(), bsr->getSenderModule()->getName());
                if (it != sta_name_5G.end()) {
                    int index = std::distance(sta_name_5G.begin(), it);         // get index
                    sta_bsr_5G[index] = bsr->getSTA_buffer_size();
                    EV << getFullName() << " BSR for " << bsr->getSenderModule()->getFullName() << " updated : " << sta_bsr_5G[index] << endl;

                    if(sta_bsr_5G[index] == 0) {        // empty buffer is reported
                        sta_tx_time_5G[index] = 0;
                    }
                    else {
                        sta_tx_time_5G[index] = 250e-6;
                    }
                }
                else {
                    EV << getFullName() << " Source of BSR NOT found!!! " << endl;
                }
            }
            delete bsr;
        }
    }
    else {
        if(strcmp(msg->getName(),"ping") == 0) {                // updating buffer size after receiving requests from ONUs
            ping *png = check_and_cast<ping *>(msg);

            if(strcmp(png->getSTA_band_pref(),"6GHz") == 0 ) {
                sta_dist_6G.push_back(png->getInter_node_dist());
                sta_bsr_6G.push_back(1000);                                // initialize with 1000B buffer size assumption
                sta_bw_6G.push_back(320e6);                                 // allocating 320 MHz as only XR connected at this band
                sta_name_6G.push_back(png->getSenderModule()->getName());
                sta_bphz_6G.push_back(12);                                  // 12 bits/s/Hz
                double datarate = 320e6*12*2;                               // spatial streams = 2
                sta_datarate_6G.push_back(datarate);
                sta_tx_time_6G.push_back(500e-6);
                sta_modules_6G.push_back(png->getSenderModule());
            }
            else if(strcmp(png->getSTA_band_pref(),"5GHz") == 0 ) {
                sta_dist_5G.push_back(png->getInter_node_dist());
                sta_bsr_5G.push_back(1000);                                // initialize with 1000B buffer size assumption
                sta_bw_5G.push_back(40e6);                                  // allocating 40 MHz as default allocation
                sta_name_5G.push_back(png->getSenderModule()->getName());
                sta_bphz_6G.push_back(10);                                  // 10 bits/s/Hz
                double datarate = 40e6*10*2;                                // spatial streams = 2
                sta_datarate_5G.push_back(datarate);
                sta_tx_time_5G.push_back(250e-6);
                sta_modules_5G.push_back(png->getSenderModule());
            }
            delete png;
        }
        else if(strcmp(msg->getName(),"scheduleTx6GHz") == 0) {
            for (int i = 0; i< sta_name_6G.size(); i++) {
                trigger_bsr *trg = new trigger_bsr("trigger_6GHz");
                trg->setByteLength(100);                                    // setting trigger packet size = 100 bytes
                double datarate_tmp = max(sta_datarate_6G[i], 320e6*12*2*(sta_bsr_6G[i]/accumulate(sta_bsr_6G.begin(), sta_bsr_6G.end(), 0.0)));
                trg->setThroughput(datarate_tmp);
                trg->setBW_alloc(sta_bw_6G[i]);
                trg->setTX_time(sta_tx_time_6G[i]);

                cModule *targetModule = sta_modules_6G[i];
                // compute delays
                simtime_t propDelay = sta_dist_6G[i] / (3e8);
                simtime_t txDuration = trg->getBitLength() / sta_datarate_6G[i];
                // send it
                sendDirect(trg, propDelay, txDuration, targetModule->gate("in"));
                EV << getFullName() << " Sent trigger to " << sta_modules_6G[i]->getFullName() << " at " << simTime();
                EV << " Datarate = " << sta_datarate_6G[i] << " and sta_tx_time_6G = " << sta_tx_time_6G[i] << endl;
            }
            // re-scheduling scheduleTx6GHz
            scheduleAt(simTime()+SimTime(wifi_ofdma_slot), scheduleTx6GHz);
        }
        else if(strcmp(msg->getName(),"scheduleTx5GHz") == 0) {
            for (int i = 0; i< sta_name_5G.size(); i++) {
                trigger_bsr *trg = new trigger_bsr("trigger_5GHz");
                trg->setByteLength(100);
                double datarate_tmp = max(sta_datarate_5G[i], 160e6*12*2*(sta_bsr_5G[i]/accumulate(sta_bsr_5G.begin(), sta_bsr_5G.end(), 0.0)));
                trg->setThroughput(datarate_tmp);
                trg->setBW_alloc(sta_bw_5G[i]);
                trg->setTX_time(sta_tx_time_5G[i]);

                cModule *targetModule = sta_modules_5G[i];
                // compute delays
                simtime_t propDelay = sta_dist_5G[i] / (3e8);
                simtime_t txDuration = trg->getBitLength() / sta_datarate_5G[i];
                // send it
                sendDirect(trg, propDelay, txDuration, targetModule->gate("in"));
                EV << getFullName() << " Sent trigger to " << sta_modules_5G[i]->getFullName() << " at " << simTime();
                EV << " Datarate = " << sta_datarate_5G[i] << " and sta_tx_time_5G = " << sta_tx_time_5G[i] << endl;
            }
            // re-scheduling scheduleTx5GHz
            scheduleAt(simTime()+SimTime(wifi_ofdma_slot), scheduleTx5GHz);
        }
        else {
            EV << getFullName() << " Some unknown cMessage has arrived at = " << simTime() << endl;
        }
    }
}




