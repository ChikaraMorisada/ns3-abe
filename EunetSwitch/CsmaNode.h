#ifndef CSMANODE_H_
#define CSMANODE_H_
#include "ns3/node.h"
#include "ns3/net-device-container.h"
#include "ns3/object-factory.h"
#include "ns3/csma-net-device.h"

class CsmaNode: public ns3::Node {
	const uint32_t nCsmaNetDevices;
	ns3::ObjectFactory deviceFactory;
	ns3::ObjectFactory queueFactory;
public:
	static ns3::TypeId GetTypeId(void);
	CsmaNode(const uint32_t n_csma_net_devices = 0);
	virtual ~CsmaNode();
protected:
	virtual void DoInitialize();
};

#endif /* CSMANODE_H_ */
