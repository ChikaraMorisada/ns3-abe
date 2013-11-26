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

CsmaChannelNode::CsmaChannelNode(const int n_devices) :
	CsmaNode(n_devices) {
	this->csmaChannelFactory.SetTypeId("ns3::CsmaChannel");
	for (unsigned i = 0; i < this->GetNDevices(); ++i) {
		ns3::Ptr<ns3::CsmaChannel>
				ptr_csma_channel =
						this->csmaChannelFactory.Create()->GetObject<
								ns3::CsmaChannel> ();
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
	// TODO !CodeTemplates.destructorstub.tododesc!
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
