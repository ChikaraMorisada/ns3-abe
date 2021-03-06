/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 The Boeing Company
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/propagation-module.h"
#include "ns3/olsr-helper.h"

NS_LOG_COMPONENT_DEFINE ("CollisionSample");

using namespace ns3;

void ReceivePacket (Ptr<Socket> socket)
{
  NS_LOG_UNCOND ("Received one packet!");
}

static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, 
                             uint32_t pktCount, Time pktInterval )
{
  if (pktCount > 0)
    {
      socket->Send (Create<Packet> (pktSize));
      Simulator::Schedule (pktInterval, &GenerateTraffic, 
                           socket, pktSize,pktCount-1, pktInterval);
    }
  else
    {
      socket->Close ();
    }
}

void myRxOkCallback(Ptr<Packet> p, double snr, WifiMode mode, enum WifiPreamble preamble){
  NS_LOG_UNCOND("myRxOkCallback, snr=" << snr);
}

void myRxErrorCallback(Ptr<const Packet> packet, double snr) {
  NS_LOG_UNCOND("myRxErrorCallback, snr=" << snr);
}


int main (int argc, char *argv[])
{
  //LogComponentEnableAll(LOG_ALL);
  std::string phyMode ("DsssRate1Mbps");
  double rss = -80;  // -dBm
  double Prss = -80;
  //double Irss = -95;
  uint32_t packetSize = 1000; // bytes
  uint32_t numPackets = 1;
  uint32_t numPackets3 = 1;
  double interval = 0.1; // seconds
  bool verbose = false;
  double offset = 91;
  
  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("rss", "received signal strength", rss);
  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
  cmd.AddValue ("numPackets", "number of packets generated", numPackets);
  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);

  cmd.Parse (argc, argv);
  // Convert to time object
  Time interPacketInterval = Seconds (interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", 
                      StringValue (phyMode));

  NodeContainer c;
  c.Create (3);

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi_helper;
  if (verbose)
    {
      wifi_helper.EnableLogComponents ();  // Turn on all Wifi logging
    }
  wifi_helper.SetStandard (WIFI_PHY_STANDARD_80211b);


  // 2. Place nodes somehow, this is required by every wireless simulation
  for (size_t i = 0; i < 3; ++i)
    {
      c.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());
    }

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (0) );
  //この閾値より大きいと受信可能
  wifiPhy.Set ("CcaMode1Threshold", DoubleValue (0.0) );
  
  // 4. Create & setup wifi channel
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // 5. Install wireless devices

  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifiMac.SetType ("ns3::AdhocWifiMac"); // use ad-hoc MAC
  //NetDeviceContainer devices = wifi_helper.Install (wifiPhy, wifiMac, c);
  wifi_helper.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi_helper.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));

  NetDeviceContainer devices;
  
  Ptr<YansWifiChannel> p_yans_wifi_channel = wifiChannel.Create();

  for (unsigned j = 0; j < 3; j++) {
    Ptr<Node> node =  c.Get(j);
    Ptr<WifiNetDevice> device = CreateObject<WifiNetDevice>();

    ObjectFactory nqos_wifi_mac_factory = ObjectFactory();
    nqos_wifi_mac_factory.SetTypeId("ns3::AdhocWifiMac");
    nqos_wifi_mac_factory.Set("QosSupported", BooleanValue(false));
    Ptr<WifiMac> p_wifi_mac = nqos_wifi_mac_factory.Create<WifiMac>();
    p_wifi_mac->ConfigureStandard(WIFI_PHY_STANDARD_80211b);
    
    ObjectFactory error_rate_model_factory = ObjectFactory();
    error_rate_model_factory.SetTypeId("ns3::NistErrorRateModel");
    Ptr<ErrorRateModel> p_error_rate_model = error_rate_model_factory.Create<ErrorRateModel>();

    ObjectFactory yans_wifi_phy_factory = ObjectFactory();
    yans_wifi_phy_factory.SetTypeId("ns3::YansWifiPhy");
    yans_wifi_phy_factory.Set ("RxGain", DoubleValue (0) );
    yans_wifi_phy_factory.Set ("CcaMode1Threshold", DoubleValue (0.0) );
#if 0
    yans_wifi_phy_factory.Set ("EnergyDetectionThreshold", DoubleValue (0.0) );
#endif
    yans_wifi_phy_factory.Set ("TxGain", DoubleValue (offset + Prss) ); 
    Ptr<YansWifiPhy> p_yans_wifi_phy = yans_wifi_phy_factory.Create<YansWifiPhy>();
    p_yans_wifi_phy->SetErrorRateModel(p_error_rate_model);
    p_yans_wifi_phy->SetChannel(p_yans_wifi_channel);
    p_yans_wifi_phy->SetMobility(node);   
    p_yans_wifi_phy->SetDevice(device);
    p_yans_wifi_phy->ConfigureStandard(WIFI_PHY_STANDARD_80211b);
    p_yans_wifi_phy->SetReceiveOkCallback(MakeCallback (&myRxOkCallback));
    p_yans_wifi_phy->SetReceiveErrorCallback(MakeCallback (&myRxErrorCallback));
     
    ObjectFactory station_manager = ObjectFactory();
    station_manager.SetTypeId("ns3::ConstantRateWifiManager");
    station_manager.Set("DataMode", StringValue(phyMode));
    station_manager.Set("ControlMode", StringValue(phyMode));
    Ptr<WifiRemoteStationManager> manager = station_manager.Create<WifiRemoteStationManager>();

    device->SetMac(p_wifi_mac);
    device->SetPhy(p_yans_wifi_phy);
    device->SetRemoteStationManager(manager);

    node->AddDevice(device);
    devices.Add(device);
  }
    
  // Note that with FixedRssLossModel, the positions below are not 
  // used for received signal strength. 
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (1.0, 0.0, 0.0));
  positionAlloc->Add (Vector (2.0, 0.0, 0.0));
  positionAlloc->Add (Vector (3.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  InternetStackHelper internet;
  internet.Install (c);
    
  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink = Socket::CreateSocket (c.Get (1), tid);
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
  recvSink->Bind (local);
  recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

  Ptr<Socket> source = Socket::CreateSocket (c.Get (0), tid);
  InetSocketAddress remote = InetSocketAddress (Ipv4Address ("255.255.255.255"), 80);
  source->SetAllowBroadcast (true);
  source->Connect (remote);

  Ptr<Socket> source3 = Socket::CreateSocket (c.Get (2), tid);
  InetSocketAddress remote3 = InetSocketAddress (Ipv4Address ("255.255.255.255"), 80);
  source3->SetAllowBroadcast (true);
  source3->Connect (remote3);
  

  // Tracing
  wifiPhy.EnablePcap ("collision-sample", devices);

  // Output what we are doing
  NS_LOG_UNCOND ("Testing " << numPackets  << " packets sent with receiver rss " << rss );
  Simulator::ScheduleWithContext (source->GetNode ()->GetId (),
                                  Seconds (2.1), &GenerateTraffic, 
                                  source, packetSize, numPackets, interPacketInterval);

  Simulator::ScheduleWithContext (source->GetNode ()->GetId (),
                                  Seconds (2.1), &GenerateTraffic, 
                                  source, packetSize, numPackets3, interPacketInterval);
    
 #if 0 
  Simulator::ScheduleWithContext (source3->GetNode ()->GetId (),
                                  Seconds (3), &GenerateTraffic, 
                                  source3, packetSize, numPackets3, interPacketInterval);
  Simulator::ScheduleWithContext (source3->GetNode ()->GetId (),
                                  Seconds (2.2), &GenerateTraffic, 
                                  source3, packetSize, numPackets3, interPacketInterval);
#endif

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

