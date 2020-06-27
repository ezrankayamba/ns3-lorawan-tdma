/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef TDMA_SENDER_H
#define TDMA_SENDER_H

#include "ns3/application.h"
#include "ns3/nstime.h"
#include "ns3/lorawan-mac.h"
#include "ns3/attribute.h"

using namespace ns3;
using namespace lorawan;

namespace ns3 {

namespace tdma {
struct TDMAParams {
	uint16_t interval;
};
class TDMASender : public Application
{
public:
	TDMASender ();
//	TDMASender (Time sendTime);
  ~TDMASender ();

  static TypeId GetTypeId (void);

  void ScheduleReach(void);

  /**
   * Send a packet using the LoraNetDevice's Send method.
   */
  void SendPacket (void);
  void SetTDMAParams(TDMAParams params);

  /**
   * Start the application by scheduling the first SendPacket event.
   */
  void StartApplication (void);

  /**
   * Stop the application.
   */
  void StopApplication (void);

private:
  uint16_t m_rate;
  uint16_t m_packet;
  double m_send_window;
  uint16_t m_tracker;
  uint16_t node_id;
  double slot;
  double toa;
  tm startTime;
  uint16_t m_interval;


  /**
   * The sending event.
   */
  EventId m_sendEvent;

  /**
   * The MAC layer of this node.
   */
  Ptr<LorawanMac> m_mac;
};

}

}

#endif /* TDMA_H */

