/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef TDMA_H
#define TDMA_H

#include <cmath>
#include <ctime>
#include <sys/time.h>
#include "ns3/attribute.h"


namespace ns3 {
	namespace tdma {

		double delta(uint16_t n_id);
		void fmt_date_tm(tm *ltm, char * date_str, double add_sec);

	}

}

#endif /* TDMA_H */

