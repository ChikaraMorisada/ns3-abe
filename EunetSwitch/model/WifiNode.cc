#define NS3_LOG_ENABLE 1
#include "ns3/log.h"
NS_LOG_COMPONENT_DEFINE("WifiNode");
#define NS3_ASSERT_ENABLE 1
#include "ns3/assert.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/nstime.h"
#include "WifiNode.h"
NS_OBJECT_ENSURE_REGISTERED(WifiNode);

ns3::TypeId WifiNode::GetTypeId(void) {
	static ns3::TypeId
			type_id =
					ns3::TypeId("WifiNode").SetParent<Base> ().AddConstructor<
							WifiNode> ();
	return type_id;
}//GetTypeId

WifiNode::WifiNode() {
}

WifiNode::~WifiNode() {
}

void WifiNode::DoInitialize() {
	auto ptr_node = ns3::Ptr<WifiNode>(this, true);
	NS_LOG_INFO("setting position to " << this->position);
	ns3::MobilityHelper mobility_helper;
	auto ptr_position_allocator =
			ns3::CreateObject<ns3::ListPositionAllocator>();
	ptr_position_allocator->Add(this->position);
	mobility_helper.SetPositionAllocator(ptr_position_allocator);
	mobility_helper.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility_helper.Install(ptr_node);

	NS_LOG_INFO("setting ssid to " << this->ssid);
	ns3::Ptr<ns3::WifiNetDevice> ptr_wifi_net_device = ptr_node->getNetDevice<
			ns3::WifiNetDevice> ();
	NS_ASSERT(ptr_wifi_net_device !=0);
	auto ptr_wifi_mac = ptr_wifi_net_device->GetMac();
	NS_ASSERT(ptr_wifi_mac != 0);
	//NS_LOG_UNCOND(ptr_wifi_mac);
	if (this->ssid.IsEqual(ns3::Ssid())) {
		NS_FATAL_ERROR("ssid is not specified. call setSsid in advance.");
	}
	ptr_wifi_mac->SetSsid(this->ssid);

	NS_LOG_INFO("calling up OnOffNode::DoInitialize");
	Base::DoInitialize();
}

void WifiNode::NotifyConstructionCompleted() {
}
