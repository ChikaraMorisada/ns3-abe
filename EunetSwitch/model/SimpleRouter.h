#ifndef SIMPLEROUTER_H_
#define SIMPLEROUTER_H_
#include "ns3/nstime.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
//#include "ns3/dce-application-helper.h"
//#include "ns3/dce-module.h"
//#include "ns3/quagga-helper.h"
//#include "CsmaChannelNode.h"
#include "CsmaInternetNode.h"
using namespace ns3;

class SimpleRouter: public CsmaInternetNode {
	//typedef CsmaChannelNode Base;
	ns3::DceApplicationHelper dceApplicationHelper;
	static const ns3::DataRate defaultlinkDataRate;
	static const ns3::TimeValue defaultlinkDelay;
	bool isNotifyConstructionCompletedCalled;
	bool isDoInitializeCalled;

public:
	const unsigned nlinkPorts;

	static ns3::TypeId GetTypeId(void);
	SimpleRouter(const unsigned n_ports = 48);
	virtual ~SimpleRouter();
	void setlinkDataRate(ns3::DataRateValue = defaultlinkDataRate);
	void setlinkDelay(ns3::TimeValue = defaultlinkDelay);
	ns3::Ptr<ns3::CsmaNetDevice>
	getLinkPort(const unsigned i_link_port);
	void connectTo(const unsigned i_link_port,
			ns3::Ptr<SimpleRouter> connect_to_device,
			const unsigned connect_i_link_port);
	void connectTo(std::string router_name);
	unsigned getUnusedlinkPort();
	void enableOspf(const unsigned i_device);

protected:
	virtual void DoInitialize();
	virtual void NotifyConstructionCompleted();
private:
	bool isConnectedToSimpleRouter(const unsigned i_port);
	void assignIpAddressToDevice(unsigned i_net_device, ns3::Ipv4Address,
			ns3::Ipv4Mask);
	std::unique_ptr<std::vector<std::string> > getAllNetworks();
}; // class SimpleRouter

#endif /* SIMPLEROUTER_H_ */
