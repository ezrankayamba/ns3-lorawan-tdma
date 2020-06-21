/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "tdma-helper.h"
#include "ns3/tdma-sender-helper.h"
#include "ns3/tdma-sender.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

namespace ns3 {

namespace tdma {


NS_LOG_COMPONENT_DEFINE ("TDMASenderHelper");

TDMASenderHelper::TDMASenderHelper ()
{
  m_factory.SetTypeId ("ns3::TDMASender");
}

TDMASenderHelper::~TDMASenderHelper ()
{
}

void
TDMASenderHelper::SetSlot(uint16_t slot)
{
	m_slot = slot;
}

void
TDMASenderHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
TDMASenderHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
TDMASenderHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  uint16_t counter(0);
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i){
	  counter++;
  }
//  NS_LOG_INFO("Counter: "<<counter);
//  Time pause = counter > 1 ? Seconds((counter - 1) * (2 + m_slot)) : Seconds(2);
//  Time pause = Seconds(30*60);

  counter=0;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i){
//	 Time delay = Seconds(0 + counter * (1 + m_slot));
     apps.Add (InstallPriv (*i));
     counter++;
  }

  return apps;
}

Ptr<Application>
TDMASenderHelper::InstallPriv (Ptr<Node> node) const
{
//  NS_LOG_FUNCTION (this << node->GetId ());

  Ptr<TDMASender> app = m_factory.Create<TDMASender> ();

//  app->SetInitialDelay (delay);
//  app->SetPause (pause);

  app->SetNode (node);
  node->AddApplication (app);

  return app;
}

}


}

