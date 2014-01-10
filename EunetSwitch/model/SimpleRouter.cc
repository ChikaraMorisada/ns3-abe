#include <memory>
#include <sstream>
#define NS3_LOG_ENABLE 1
#include "ns3/log.h"
NS_LOG_COMPONENT_DEFINE("SimpleRouter");
#define NS3_ASSERT_ENABLE 1
#include "ns3/assert.h"
#include "ns3/type-id.h"
#include "ns3/bridge-helper.h"
#include "ns3/dce-module.h"
//#include "ns3/dce-application-helper.h"
#include "ns3/quagga-helper.h"
#include "ns3/names.h"
#include "SimpleRouter.h"

NS_OBJECT_ENSURE_REGISTERED( SimpleRouter);

const ns3::DataRate SimpleRouter::defaultlinkDataRate("1000000000bps");
const ns3::TimeValue SimpleRouter::defaultlinkDelay(ns3::MilliSeconds(1));

ns3::TypeId SimpleRouter::GetTypeId(void) {
	static ns3::TypeId type_id = ns3::TypeId("SimpleRouter").SetParent<
			CsmaInternetNode> ().AddConstructor<SimpleRouter> ();
	return type_id;
}//GetTypeId


SimpleRouter::SimpleRouter(const unsigned n_ports) :
	/*CsmaChannelNode(n_ports, defaultlinkDataRate, defaultlinkDelay),*/
	isNotifyConstructionCompletedCalled(false), isDoInitializeCalled(false),
			nlinkPorts(n_ports) {
	//	NS_LOG_INFO("constructing SimpleSwitch");
	//	for (unsigned i = 0; i < nlinkPorts; ++i) {
	//		NS_LOG_INFO("investigating port " << i);
	//		NS_ASSERT(!this->isConnectedToSimpleRouter(i));
	//	}
}

SimpleRouter::~SimpleRouter() {
	NS_ASSERT(this->isDoInitializeCalled);
	NS_ASSERT(this->isNotifyConstructionCompletedCalled);
}

void SimpleRouter::setlinkDataRate(ns3::DataRateValue data_rate) {
	for (unsigned i = 0; i < nlinkPorts; ++i) {
		this->setDataRate(data_rate, i);
	}
}

void SimpleRouter::setlinkDelay(ns3::TimeValue delay) {
	for (unsigned i = 0; i < nlinkPorts; ++i) {
		this->setDelay(delay, i);
	}
}

ns3::Ptr<ns3::CsmaNetDevice> SimpleRouter::getLinkPort(const unsigned i_port) {
	NS_ASSERT(i_port < nlinkPorts);
	auto p = this->getCsmaNetDevice(i_port);
	return p;
}//getlinkPort

void SimpleRouter::connectTo(const unsigned i_link_port,
		ns3::Ptr<SimpleRouter> connect_router,
		const unsigned connect_i_link_port) {
	this->bring(i_link_port, connect_router, connect_i_link_port);
	NS_ASSERT(this->isConnectedToSimpleRouter(i_link_port));
	NS_ASSERT(connect_router->isConnectedToSimpleRouter(connect_i_link_port));
}//connectTo

void SimpleRouter::connectTo(std::string connect_router_name) {
	ns3::Ptr<ns3::Node> ptr_node = ns3::Names::Find<ns3::Node>(
			connect_router_name);
	auto ptr_connect_router = ptr_node->GetObject<SimpleRouter> ();
	const unsigned unused_link_port = this->getUnusedlinkPort();
	const unsigned connect_unused_link_port =
			ptr_connect_router->getUnusedlinkPort();
	this->connectTo(unused_link_port, ptr_connect_router,
			connect_unused_link_port);
}//connectTo

unsigned SimpleRouter::getUnusedlinkPort() {
	for (unsigned i = 0; i < this->nlinkPorts; ++i) {
		if (!this->isConnectedToSimpleRouter(i)) {
			return i;
		}//if
	}//for
	NS_FATAL_ERROR("no unused downlink port");
}

void SimpleRouter::DoInitialize() {
	NS_ASSERT(!this->isDoInitializeCalled);
	this->isDoInitializeCalled = true;
	NS_LOG_INFO("just calling up CsmaChannelNode::DoInitialize");
	CsmaInternetNode::DoInitialize();
	this->setlinkDataRate(defaultlinkDataRate);
	this->setlinkDelay(defaultlinkDelay);
}

void SimpleRouter::NotifyConstructionCompleted() {
	NS_ASSERT(!this->isNotifyConstructionCompletedCalled);
	this->isNotifyConstructionCompletedCalled = true;
	NS_LOG_INFO("just calling up CsmaChannelNode::NotifyConstructionCompleted");
	CsmaInternetNode::NotifyConstructionCompleted();
	NS_LOG_INFO("CsmaChannelNode::NotifyConstructionCompleted finished");
	ns3::Ptr<SimpleRouter> ptr_this(this, true);
	NS_ASSERT(ptr_this != 0);
	//const auto n_devices_before = ptr_this->GetNDevices();
	//NS_ASSERT(n_devices_before > 0);
	//NS_LOG_INFO("node " << this->GetId() << " has " << n_devices_before << " device(s)");

	//NS_LOG_INFO("preparing InternetStackHelper");
	//ns3::InternetStackHelper internet_stack_helper; // IPv4 is required for GlobalRouteMan
	NS_LOG_INFO("preparing Ipv4DceRoutingHelper");
	ns3::Ipv4DceRoutingHelper ipv4_dce_routing_helper;
	auto ipv4_routing_protocol = ipv4_dce_routing_helper.Create(ptr_this);
	//ns3::Ipv4GlobalRoutingHelper ipv4_global_routing_helper;
	//NS_LOG_INFO("setting Ipv4DceRoutingHelper to InternetStackHelper");
	//internet_stack_helper.SetRoutingHelper(ipv4_dce_routing_helper);
	//NS_LOG_INFO("installing internet stack on the node");
	//internet_stack_helper.Install(ptr_this);
	//NS_LOG_INFO("internet stack was installed on the node");
	auto ptr_ipv4 = ptr_this->GetObject<ns3::Ipv4> ();
	ptr_ipv4->SetRoutingProtocol(ipv4_routing_protocol);
	NS_LOG_DEBUG("Ipv4DceRoutingProtocol was installed");

	//const auto n_devices_after = ptr_this->GetNDevices();
	//NS_ASSERT(n_devices_before +1 == n_devices_after);
	//NS_ASSERT(this->GetDevice(n_devices_before)->GetObject<ns3::LoopbackNetDevice>(ns3::LoopbackNetDevice::GetTypeId()));

	//this->assignIpAddressToDevice();
	//Ipv4AddressHelper ipv4AddrHelper;
	//ipv4AddrHelper.SetBase("10.0.0.0", "255.255.255.0");
	//Ipv4InterfaceContainer interfaces = ipv4AddrHelper.Assign(
	//		this->GetDevice(0));
	//NS_LOG_INFO("populating global routing table");;
	//Ipv4GlobalRoutingHelper::PopulateRoutingTables();

	NS_LOG_INFO("preparing DceManagerHelper");
	ns3::DceManagerHelper dce_manager_helper;
	dce_manager_helper.SetNetworkStack("ns3::Ns3SocketFdFactory");
	NS_LOG_INFO("installing with DceManagerHelper");
	dce_manager_helper.Install(ns3::NodeContainer(ptr_this));
	NS_LOG_INFO("DceManagerHelper was installed.");
}//NotifyConstructionCompleted

bool SimpleRouter::isConnectedToSimpleRouter(const unsigned i_port) {
	ns3::Ptr<ns3::CsmaChannel> ptr_csma_channel = this->getCsmaChannel(i_port);
	NS_LOG_INFO("SimpleRouter "<< this->GetId() << " port " << i_port
			<< " channel is attached to " << ptr_csma_channel->GetNDevices()<< " devices ");
	for (unsigned i = 0; i < ptr_csma_channel->GetNDevices(); ++i) {
		NS_LOG_INFO("inspecting port " << i);
		ns3::Ptr<ns3::NetDevice> ptr_net_device =
				ptr_csma_channel->GetDevice(i);
		NS_LOG_INFO("expecting NetDevice .. " << ptr_net_device->GetTypeId());
		ns3::Ptr<ns3::Node> ptr_node = ptr_net_device->GetNode();
		NS_LOG_INFO("expecting Node .. " << ptr_node->GetTypeId());
		if (ptr_node->GetId() == this->GetId()) {
			continue;
		}//if
		ns3::Ptr<SimpleRouter> ptr_simple_router = ptr_node->GetObject<
				SimpleRouter> ();
		NS_LOG_INFO("expecting SimpleRouter .. " << ptr_simple_router->GetTypeId());
		if (ptr_simple_router) {
			return true;
		}//if
	}//for
	return false;
}//isConnectedToSimpleRouter

std::unique_ptr<std::vector<std::string> > SimpleRouter::getAllNetworks() {
	std::unique_ptr<std::vector<std::string> > p_networks(new std::vector<
			std::string>());
	ns3::Ptr<SimpleRouter> ptr_this(this, true);
	auto ptr_ipv4 = ptr_this->GetObject<ns3::Ipv4> ();
	for (unsigned i = 0; i < this->GetNDevices(); ++i) {
		auto p_net_device = this->GetDevice(i);
		auto p_loopback_device =
				p_net_device->GetObject<ns3::LoopbackNetDevice> ();
		if (p_loopback_device)
			continue;
		auto i_interface = ptr_ipv4->GetInterfaceForDevice(p_net_device);
		NS_ASSERT_MSG(i_interface != -1, "IP address is not assigned yet.");
		if (i_interface == -1)
			continue;
		auto n_addresses = ptr_ipv4->GetNAddresses(i_interface);
		for (unsigned j = 0; n_addresses > j; ++j) {
			auto ipv4_interface_address = ptr_ipv4->GetAddress(i_interface, j);
			NS_LOG_INFO(ipv4_interface_address);
			auto ipv4_address = ipv4_interface_address.GetLocal();
			auto ipv4_mask = ipv4_interface_address.GetMask();
			ipv4_address.CombineMask(ipv4_mask);
			std::ostringstream oss;
			oss << ipv4_address;
			auto s = oss.str();
			NS_LOG_INFO("network address " << s);
			p_networks->push_back(s);
		}//for
	}//for
	return p_networks;
}

void SimpleRouter::enableOspf(const unsigned i_net_device) {
	NS_LOG_DEBUG("enabling Quagga OSPF routing daemon");
	const auto ipv4_interface_address = this->getIpv4InterfaceAddress(
			i_net_device);
	ns3::QuaggaHelper quagga;
	//auto all_networks = this->getAllNetworks();
	//for (auto i = all_networks->begin(); i != all_networks->end(); ++i) {
	//	quagga.EnableOspf(ns3::NodeContainer(ptr_this), (*i).c_str());
	//}//for
	const auto ipv4_address = ipv4_interface_address.GetLocal();
	const auto ipv4_mask = ipv4_interface_address.GetMask();
	const auto ipv4_network_address = ipv4_address.CombineMask(ipv4_mask);
	std::ostringstream oss;
	ipv4_network_address.Print(oss);
	oss << '/' << ipv4_mask.GetPrefixLength();
	NS_LOG_DEBUG("enabling Quagga OSPF to network " << oss.str());
	ns3::Ptr<SimpleRouter> ptr_this(this, true);
	quagga.EnableOspf(ns3::NodeContainer(ptr_this), oss.str().c_str());
	NS_LOG_DEBUG("enabling Quagga OSPF debug");
	quagga.EnableOspfDebug(ns3::NodeContainer(ptr_this));
	NS_LOG_DEBUG("enabling Zebra debug");
	quagga.EnableZebraDebug(ns3::NodeContainer(ptr_this));
	NS_LOG_DEBUG("installing Quagga OSPF routing daemon");
	quagga.Install(ns3::NodeContainer(ptr_this));
	NS_LOG_INFO("Quagga OSPF routing daemon was installed.");
}//enableOspf
