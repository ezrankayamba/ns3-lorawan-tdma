/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "tdma.h"
#include "ns3/tdma-sender.h"
#include "ns3/class-a-end-device-lorawan-mac.h"
#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/lora-net-device.h"
#include "ns3/simulator.h"

namespace ns3 {

namespace tdma {
NS_LOG_COMPONENT_DEFINE ("TDMASender");

NS_OBJECT_ENSURE_REGISTERED (TDMASender);

TypeId
TDMASender::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TDMASender")
    .SetParent<Application> ()
    .AddConstructor<TDMASender> ()
    .SetGroupName ("tdma");
  return tid;
}

TDMASender::TDMASender ()
{
//  NS_LOG_FUNCTION_NOARGS ();
  m_rate=500;
  m_packet=0;
  m_slot=2;
  time_t t_time(time(NULL));
  startTime = *localtime(&t_time);
  m_pause = Seconds(30*60);
}

TDMASender::~TDMASender ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
TDMASender::ScheduleReach(void){
//	NS_LOG_FUNCTION_NOARGS ();
	tm ltm = startTime;
	double sim_tm=Simulator::Now().GetSeconds();
	ltm.tm_sec += sim_tm;
	char now_str[40];
	fmt_date_tm(&ltm, now_str, 0);
	ltm.tm_sec -= sim_tm;

	Ptr<Packet> packet = Create<Packet> (m_packet);
	m_mac->Send (packet);
	NS_LOG_INFO("Sent: "<<this->m_packet <<", At: " << now_str);

	Simulator::Cancel (m_sendEvent);
	m_sendEvent = Simulator::Schedule (m_pause, &TDMASender::SendPacket, this);
}

void
TDMASender::SendPacket (void)
{
//  NS_LOG_FUNCTION_NOARGS ();
  tm ltm = startTime;
  Ptr<RandomVariableStream> rv = CreateObjectWithAttributes<UniformRandomVariable> ("Min", DoubleValue (30), "Max", DoubleValue (50));
  this->m_packet = rv->GetInteger();
//  NS_LOG_INFO("Ready to send: " << this->m_packet << ", at rate(/s): " << this->m_rate);

  this->m_tracker = 0;
  double sim_tm=Simulator::Now().GetSeconds();
  ltm.tm_sec += sim_tm;
  char date_str[40];
  char now_str[40];
  fmt_date_tm(&ltm, now_str, 0);
  fmt_date_tm(&ltm, date_str, slot);
  ltm.tm_sec -= sim_tm;
  NS_LOG_INFO("Now: "<<now_str <<", Next: " << date_str);
  Simulator::Cancel (m_sendEvent);
  m_sendEvent = Simulator::Schedule (Seconds(slot), &TDMASender::ScheduleReach, this);
}

void
TDMASender::StartApplication (void)
{
  tm ltm = startTime;
  NS_LOG_FUNCTION (this);
  slot = delta((m_node->GetId()));// in seconds
//  NS_LOG_INFO("Start Now: "<<Simulator::Now().GetSeconds());

  char now_str[40];
  fmt_date_tm(&ltm, now_str, 0);
  NS_LOG_INFO("Starting Application with DELTA of " << slot << " seconds, At: " <<now_str);

  // Make sure we have a MAC layer
  if (m_mac == 0)
    {
      // Assumes there's only one device
      Ptr<LoraNetDevice> loraNetDevice = m_node->GetDevice (0)->GetObject<LoraNetDevice> ();

      m_mac = loraNetDevice->GetMac ();
      NS_ASSERT (m_mac != 0);
    }


  // Schedule the next SendPacket event
  Simulator::Cancel (m_sendEvent);
  m_sendEvent = Simulator::Schedule (Seconds(0), &TDMASender::SendPacket, this);
}

void
TDMASender::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_INFO("Stopping Application");
  Simulator::Cancel (m_sendEvent);
}

}


}

