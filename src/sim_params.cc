/*
 * sim_params.cc
 *
 *  Created on: 30 July 2025
 *      Author: mondals
 */

#include <string.h>
#include <math.h>
#include <omnetpp.h>
#include "sim_params.h"

using namespace std;
using namespace omnetpp;

double olt_onu_distance = 20;                                           // OLT-ONU distance (km)
double light_speed = 2e5;                                               // speed of light in fiber 2 x 10^5 km/s
double ext_pon_link_datarate = 50e9;                                    // External PON link datarate = 50 Gbps
double int_pon_link_datarate = 10e9;                                    // Internal PON link datarate = 10 Gbps
double max_polling_cycle = 125e-6;                                      // maximum polling cycle duration

int const pkt_sz_min = 64;                                              // Ethernet packet size - minimum (bytes)
int const pkt_sz_max = 1542;                                            // Ethernet packet size - maximum (bytes)
//int const pkt_sz_max = 1000;                                          // for testing 1:16 1-GPON without fragmentation
int pkt_sz_avg = ceil((pkt_sz_min + pkt_sz_max)/2);                     // Average packet size (bytes)

double onu_buffer_capacity = 100e9;                                     // ONU buffer capacity (bytes)
double sfu_buffer_capacity = 50e9;                                      // SFU buffer capacity (bytes)
double T_guard = 1e-6;                                                  // guard time for each ONU

double wifi_ofdma_slot = 1e-3;                                          // time duration of each OFDMA slot
double wifi_sifs_time = 16e-6;                                          // SIFS time duration
double total_5GHz_bandwidth = 160e6;                                    // total 5 GHz bandwidth
double total_6GHz_bandwidth = 320e6;                                    // total 6 GHz bandwidth
int wifi_stas_connected = 5;                                            // No. of STAs connected to each WAP




