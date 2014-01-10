#ifndef CSMACHANNELNODE_H_
#define CSMACHANNELNODE_H_
#include "ns3/node.h"
#include "CsmaNode.h"
#include "ns3/csma-channel.h"

class CsmaChannelNode: public CsmaNode {
	bool isNotifyConstructionCompletedCalled;
	bool isDoInitializeCalled;
	ns3::ObjectFactory csmaChannelFactory;
public:
	static ns3::TypeId GetTypeId(void);
	CsmaChannelNode(const int n_devices = 1, ns3::DataRateValue data_rate =
			ns3::DataRate(0xffffffff), ns3::TimeValue = ns3::Seconds(0)); // TODO: should support multiple devices
	virtual ~CsmaChannelNode();
	void bring(ns3::Ptr<CsmaNode> ptr_their_csma_node,
			const unsigned i_their_csma_device = 0);
	void bring(const unsigned i_our_csma_device,
			ns3::Ptr<CsmaNode> ptr_their_csma_node,
			const unsigned i_their_csma_device = 0);
	ns3::Ptr<ns3::CsmaChannel> getCsmaChannel(unsigned i_channel);
	void setDataRate(ns3::DataRateValue&, const unsigned i_channel = 0);
	void setDelay(ns3::TimeValue&, const unsigned i_channel = 0);
protected:
	virtual void DoInitialize();
	virtual void NotifyConstructionCompleted();
};

#endif /* CSMACHANNELNODE_H_ */
