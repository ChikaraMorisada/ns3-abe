#define NS3_LOG_ENABLE 1
#include "ns3/log.h"
#define NS3_ASSERT_ENABLE 1
#include "ns3/assert.h"
#include "ns3/csma-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/mobility-helper.h"
NS_LOG_COMPONENT_DEFINE("EunetTerminals");
#include "EunetTerminal.h"
#include "EunetTerminals.h"

EunetTerminals::EunetTerminals(const unsigned n_terminals) {
	NS_LOG_INFO("constructing EunetTerminals with " << n_terminals << "terminals");
	ns3::ObjectFactory object_factory;
	object_factory.SetTypeId("EunetTerminal");
	for (unsigned i = 0; i < n_terminals; ++i) {
		NS_LOG_INFO("creating EunetTerminal node " << i);
		ns3::Ptr<EunetTerminal> ptr_eunet_terminal = object_factory.Create<
				EunetTerminal> ();
		NS_LOG_INFO(ptr_eunet_terminal->GetNDevices() << " device(s)");
		//auto ptr_eunet_terminal = object_factory.Create<EunetTerminal> ();
		//ns3::Ptr<ns3::Node> ptr_eunet_terminal(
		//		ns3::CreateObject<EunetTerminal>());
		NS_ASSERT(ptr_eunet_terminal->GetNDevices()==2);
		//ptr_eunet_terminal->assignAddress(ipv4_address_helper);
		this->Add(ptr_eunet_terminal);
	}//for
	NS_ASSERT(n_terminals == this->GetN());
#if 0
	for (unsigned i = 0; i < this->GetN(); ++i) {
		NS_LOG_INFO("getting source EunetTerminal");
		auto ptr_source = this->terminals.Get(i)->GetObject<EunetTerminal> ();
		NS_LOG_INFO("getting remote EunetTerminal");
		auto ptr_remote = this->terminals.Get(0)->GetObject<EunetTerminal> ();
		NS_LOG_INFO("setting remote EunetTerminal");
		ptr_source->setRemote(ptr_remote);
		ptr_source->startAt(ns3::Seconds(0.0));
		ptr_source->stopAt(ns3::Seconds(2.0));
	}
#endif

	ns3::MobilityHelper mobility_helper;
	mobility_helper.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility_helper.Install(*this);

}// a constructor


void EunetTerminals::assignAddresses(const char* network_address,
		const char* netmask) {
	ns3::Ipv4Address ipv4_address(network_address);
	ns3::Ipv4Mask ipv4_mask(netmask);
	ns3::Ipv4AddressHelper ipv4_address_helper(ipv4_address, ipv4_mask);
	this->assignAddresses(ipv4_address_helper);
}//assignAddresses

void EunetTerminals::assignAddresses(
		ns3::Ipv4AddressHelper& ipv4_address_helper) {
	for (unsigned i = 0; i < this->GetN(); ++i) {
		NS_ASSERT(this->Get(i)-> GetNDevices()==2);
		this->Get(i)->assignAddress(ipv4_address_helper);
	}//for
}//assignAddresses

EunetTerminals::~EunetTerminals() {
	// TODO !CodeTemplates.destructorstub.tododesc!
}

ns3::Ptr<EunetTerminal> EunetTerminals::Get(const int i_eunet_terminal) {
	auto ptr = Base::Get(i_eunet_terminal)->GetObject<EunetTerminal> ();
	NS_ASSERT(ptr->GetNDevices()==2);
	return ptr;
}

ns3::NetDeviceContainer EunetTerminals::getDevicesAll() {
	ns3::NetDeviceContainer net_device_container;
	for (unsigned i = 0; i < this->GetN(); ++i) {
		//auto ptr_eunet_terminal = getEunetTerminal(i);
		//NS_ASSERT(ptr_eunet_terminal->GetNDevices()==1);
		//auto ptr_net_device = ptr_eunet_terminal->GetDevice(0);
		net_device_container.Add(this->Get(i)->getCsmaNetDevice());
	}//for
	return net_device_container;
}//getDevicesAll

void EunetTerminals::logTotalRx(ns3::LogLevel log_level) {
	for (auto i = this->Begin(); i != this->End(); ++i) {
		(*i)->GetObject<EunetTerminal> ()-> logTotalRx(log_level);
	}
}

void EunetTerminals::bringAtoB(const unsigned i_eunet_terminal_a,
		const unsigned i_eunet_terminal_b) {
	NS_ASSERT(i_eunet_terminal_a < this->GetN());
	NS_ASSERT(i_eunet_terminal_b < this->GetN());

	NS_LOG_INFO("getting terminal " << i_eunet_terminal_a << " of " << this->GetN());
	auto a = this->Get(i_eunet_terminal_a);
	NS_LOG_INFO("getting terminal " << i_eunet_terminal_b << " of " << this->GetN());
	auto b = this->Get(i_eunet_terminal_b);
	NS_LOG_INFO("bringing node " << a->GetId() << " into CSMA domain of node " << b->GetId());
	b->bring(a);
}

void EunetTerminals::setRemoteOfAtoB(const unsigned i_eunet_terminal_a,
		const unsigned i_eunet_terminal_b) {
	auto a = this->Get(i_eunet_terminal_a);
	auto b = this->Get(i_eunet_terminal_b);
	a->setRemote<ns3::CsmaNetDevice> (b);
}
