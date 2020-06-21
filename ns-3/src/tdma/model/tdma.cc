/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "tdma.h"

namespace ns3 {

	namespace tdma {

		void fmt_date_tm(tm *ltm, char * date_str, double add_sec){
//			tm *ltm = localtime(&tm_in);
			ltm->tm_sec += add_sec;   // add 50 seconds to the time
			mktime( ltm);
			sprintf(date_str, "%04d-%02d-%02d %02d:%02d:%02d", (1900 + ltm->tm_year), ltm->tm_mon+1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
		}

		double delta(uint16_t n_id){
		    double gt=6;//Gt 6ms (Guard time)

		    double n_pre=10.5; //Preamble length, CRS, errors, other meta data (symbols); Specify during simulation
		    uint16_t sf=7; //Affect bandwidth, trans rate, range (distance) [7..12] -> Make it random, device can pick any
		    double bw=125*1000; //Bandwidth; 125, 250, 500 kHz; Specify during simulation
		    double t_sym=(pow(2, sf))/bw; // Duration of simbol
		    double r_sym=1/t_sym; //Symbol rate

		    uint16_t pl=50; //Payload length [pre][pl] in bytes [30..50]
		    uint16_t crc=1; //Cyclic redundancy check flag [1-disable or 0-enable]; Specify during simulation
		    uint16_t h=1; //Header flag [1-disable or 0-enable]
		    uint16_t de=0; //Data rate optimizer not enabled; [1-enabled or 0-disabled]
		    double cr=4/5; //[1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]; Specify during simulation

		    double t1=(n_pre+4.25)*1/r_sym;
		    double t2=(8*pl - 4*sf + 28 + 16*crc - 20*h)/(4*(sf - 2*de));
		    double t3=(ceil(t2) * (cr + 4));
		    double toa=t1 + (8 + fmax(t3, 0) * 1/r_sym);//Time on Air for the packets
		    //cout << "TOA: "<< toa << endl;
		    return ((toa * 1000 + gt) * n_id)/1000;
		}

	}

}

