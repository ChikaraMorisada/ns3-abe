#ifndef CSMAINTERNETNODE_H_
#define CSMAINTERNETNODE_H_
#include "ns3/ipv4-address-helper.h"
#include "CsmaChannelNode.h"

class CsmaInternetNode: public CsmaChannelNode {
	typedef CsmaChannelNode Base;
public:
	static ns3::TypeId GetTypeId(void);
	CsmaInternetNode(const int n_devices = 1);
	virtual ~CsmaInternetNode();
	ns3::Ipv4Address getCsmaNetDeviceAddress();

	template<class T>
	ns3::Ipv4Address getAddress();

	template<class T>
	void setAddress(ns3::Ipv4AddressHelper& ipv4_address_helper);
	void assignAddress(ns3::Ipv4AddressHelper&);
protected:
	virtual void NotifyConstructionCompleted();
};

template<class T>
ns3::Ipv4Address CsmaInternetNode::getAddress() {
	NS_ASSERT(T::GetInstanceTypeId().IsChildOf(ns3::NetDevice::GetTypeId()));
	//this->logAllDevices(ns3::LOG_LEVEL_INFO);
	const unsigned n_devices_before = this->GetNDevices();
	NS_ASSERT(this->GetNDevices()>=2);
	ns3::Ptr<ns3::Ipv4> ptr_ipv4 = this->GetObject<ns3::Ipv4> ();
	ns3::Ptr<T> ptr_net_device = this->getNetDevice<T> ();
	NS_ASSERT(ptr_net_device != 0);
	const int n_interface = ptr_ipv4->GetInterfaceForDevice(ptr_net_device);
	NS_ASSERT(n_interface != -1);
	ns3::Ipv4InterfaceAddress ipv4_interface_address = ptr_ipv4->GetAddress(
			n_interface, 0);
	ns3::Ipv4Address ipv4_address = ipv4_interface_address.GetLocal();
	NS_ASSERT(this->GetNDevices()==n_devices_before);
	return ipv4_address;
}

template<class T>
void CsmaInternetNode::setAddress(ns3::Ipv4AddressHelper& ipv4_address_helper) {
	NS_ASSERT(this->GetNDevices()==2);
	ipv4_address_helper.Assign(
			ns3::NetDeviceContainer(this->getNetDevice<T> ()));
	//this->setRemote(this);
	NS_LOG_INFO(this->getAddress<T> () << " node " << this->GetId());
}

#endif /* CSMAINTERNETNODE_H_ */
