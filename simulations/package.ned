//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

simple Background_Device
{
    parameters:
        @display("i=device/pc");
        double wap_distance @unit(m) = uniform(0.1m,5m);
        double throughput = uniform(3e9,5e9);
        double ber = default(1e-6);

        double load = default(0.3);																	// this will vary as 0.1:0.1:1
        double dataRate = default((50e9-(40e6+53.33e3+1.2e6+1.2e6)*16*4)/(16*8*3));				//max datarate in bps
        //double dataRate = default(50e9/(16*8*3));
        //double dataRate = default(100e6);

        //@signal[generation](type="double");
        //@statistic[pkt_interval](title="packet inter-arrival time"; source="generation"; record=vector,stats; interpolationmode=none);
    gates:
        input in @directIn;
        //output out;
}

simple XR_Device
{
    parameters:
        @display("i=device/xr");
        double wap_distance @unit(m) = uniform(0.1m,5m);
        double throughput = uniform(5e9,10e9);
        double ber = default(1e-6);

        double frameRate = default(60);		            // default framerate of XR = 60 fps (can be 90, 120 fps)
        double dataRate = default(40e6);				// for 2K@60fps = 40 Mbps, for 4K@60fps = 90 Mbps, for 8K@60fps = 360 Mbps, for 16K@60 fps = 440 Mbps

        //@signal[generation](type="double");
        //@statistic[pkt_interval](title="packet inter-arrival time"; source="generation"; record=vector,stats; interpolationmode=none);

    gates:
        input in @directIn;
        //output out;
}

simple HMD_Device
{
    parameters:
        @display("i=block/user");
        double wap_distance @unit(m) = uniform(0.1m,5m);
        double throughput = uniform(3e9,5e9);
        double ber = default(1e-6);

        double meanPacketSize = default(100);			    // very small value - assuming to be 100 Bytes 
        double sampleRate = default(1/15);				    // default inter-sample time = 15 ms

        //@signal[generation](type="double");
        //@statistic[pkt_interval](title="packet inter-arrival time"; source="generation"; record=vector,stats; interpolationmode=none);

    gates:
        input in @directIn;
        //output out;
}

simple Control_Device
{
    parameters:
        @display("i=device/gloves");
        double wap_distance @unit(m) = uniform(0.1m,5m);
        double throughput = uniform(3e9,5e9);
        double ber = default(1e-6);

        double meanPacketSize = default(1500);			    // very small value - assuming to be 1500 Bytes 
        double sampleRate = default(1/10);				    // default inter-sample time = 11 ms following Gaussian distribution

        //@signal[generation](type="double");
        //@statistic[pkt_interval](title="packet inter-arrival time"; source="generation"; record=vector,stats; interpolationmode=none);

    gates:
        input in @directIn;
        //output out;
}

simple Haptic_Device
{
    parameters:
        @display("i=device/robot_arm");
        double wap_distance @unit(m) = uniform(0.1m,5m);
        double throughput = uniform(3e9,5e9);
        double ber = default(1e-6);

        double meanPacketSize = default(1000);			    // very small value - assuming to be 100 Bytes 
        double sampleRate = default(1/10);				    // default inter-sample time = 11 ms following Generalized Pareto distribution

        //@signal[generation](type="double");
        //@statistic[pkt_interval](title="packet inter-arrival time"; source="generation"; record=vector,stats; interpolationmode=none);

    gates:
        input in @directIn;
        //output out;
}

simple WiFi_AP
{
    parameters:
        @display("i=device/accesspoint");
        double wap_distance @unit(m) = uniform(0.1m,5m);
        double throughput = uniform(3e9,5e9);

    gates:
        input Src_in @directIn;
}

network FTTR_WiFi
{
    parameters:
        int NumberOfWAPs = default(2);
        int NumberOfXRs = int(this.NumberOfWAPs/2);

    submodules:
        waps[this.NumberOfWAPs]: WiFi_AP {
            @display("p=164,200,c;r=90");
        }
        xrs[this.NumberOfXRs]: XR_Device {
            @display("p=350,54,c");
        }
        hmds[this.NumberOfXRs]: HMD_Device {
            @display("p=510,186,c");
        }
        controls[this.NumberOfXRs]: Control_Device {
            @display("p=596,260,c");
        }
        haptics[this.NumberOfXRs]: Haptic_Device {
            @display("p=423,120,c");
        }
        bkgs1[this.NumberOfWAPs]: Background_Device {
            @display("p=678,337,c");
        }
        bkgs2[this.NumberOfWAPs]: Background_Device {
            @display("p=751,399,c");
        }
        bkgs3[this.NumberOfWAPs]: Background_Device {
            @display("p=830,460,c");
        }
}









