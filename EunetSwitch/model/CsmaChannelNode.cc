#define NS3_LOG_ENABLE 1
#include "ns3/log.h"
NS_LOG_COMPONENT_DEFINE("CsmaChannelNode");
#define NS3_ASSERT_ENABLE 1
#include "ns3/assert.h"
#include "ns3/queue.h"
#include "ns3/node.h"
#include "CsmaNode.h"
#include "CsmaChannelNode.h"
NS_OBJECT_ENSURE_REGISTERED(CsmaChannelNode);

ns3::TypeId CsmaChannelNode::GetTypeId(void) {
	static ns3::TypeId type_id = ns3::TypeId("CsmaChannelNode").SetParent<
			CsmaNode> ().AddConstructor<CsmaChannelNode> ();
	return type_id;
}//GetTypeId

CsmaChannelNode::CsmaChannelNode(const int n_devices,
		ns3::DataRateValue data_rate, ns3::TimeValue delay) :
	CsmaNode(n_devices), isNotifyConstructionCompletedCalled(false),
			isDoInitializeCalled(false) {
	NS_LOG_INFO("constructing CsmaChannelNode with " << n_devices << " devices");
	this->csmaChannelFactory.SetTypeId("ns3::CsmaChannel");
	this->csmaChannelFactory.Set("DataRate", data_rate);
	this->csmaChannelFactory.Set("Delay", delay);
	for (unsigned i = 0; i < this->GetNDevices(); ++i) {
		ns3::Ptr<ns3::CsmaChannel>
				ptr_csma_channel =
						this->csmaChannelFactory.Create()->GetObject<
								ns3::CsmaChannel> ();
		NS_LOG_INFO("attaching a channel to device #" << i);
		this->getCsmaNetDevice(i)->Attach(ptr_csma_channel);
	}//for
}

ns3::Ptr<ns3::CsmaChannel> CsmaChannelNode::getCsmaChannel(unsigned i_channel) {
	NS_ASSERT(i_channel < this->GetNDevices());
	auto ptr_csma_net_device = this->getCsmaNetDevice(i_channel);
	auto ptr_channel = ptr_csma_net_device->GetChannel();
	auto ptr_csma_channel = ptr_channel->GetObject<ns3::CsmaChannel> ();
	return ptr_csma_channel;
}

CsmaChannelNode::~CsmaChannelNode() {
	NS_ASSERT(this->isNotifyConstructionCompletedCalled);
	NS_ASSERT(this->isDoInitializeCalled);
}

void CsmaChannelNode::bring(ns3::Ptr<CsmaNode> ptr_newcomer,
		const unsigned i_their_csma_device) {
	this->bring(0, ptr_newcomer, i_their_csma_device);
}

void CsmaChannelNode::bring(const unsigned i_our_csma_device,
		ns3::Ptr<CsmaNode> ptr_their_node, const unsigned i_their_csma_device) {
	auto ptr_our_csma_channel = this->getCsmaChannel(i_our_csma_device);
	auto their_csma_device = ptr_their_node->getCsmaNetDevice(
			i_their_csma_device);
	//auto ptr_newcomer_device = ptr_newcomer->getCsmaNetDevice();
	their_csma_device->Attach(ptr_our_csma_channel);
}

void CsmaChannelNode::setDataRate(ns3::DataRateValue& data_rate,
		const unsigned i_channel) {
	auto ptr_csma_channel = this->getCsmaChannel(i_channel);
	ptr_csma_channel->SetAttribute("DataRate", data_rate);
}

void CsmaChannelNode::setDelay(ns3::TimeValue& delay, const unsigned i_channel) {
	auto p = this->getCsmaChannel(i_channel);
	p->SetAttribute("Delay", delay);
}

void CsmaChannelNode::DoInitialize() {
	NS_ASSERT(!this->isDoInitializeCalled);
	this->isDoInitializeCalled = true;
	NS_LOG_INFO("just calling up CsmaNode::DoInitialize");
	CsmaNode::DoInitialize();
}
void CsmaChannelNode::NotifyConstructionCompleted() {
	NS_ASSERT(!this->isNotifyConstructionCompletedCalled);
	this->isNotifyConstructionCompletedCalled = true;
	NS_LOG_INFO("just calling up CsmaNode::NotifyConstructionCompleted");
	CsmaNode::NotifyConstructionCompleted();
}
