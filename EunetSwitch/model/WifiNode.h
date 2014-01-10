#ifndef WIFINODE_H_
#define WIFINODE_H_
#include "OnOffNode.h"

class WifiNode: public OnOffNode {
	typedef OnOffNode Base;
public:
	static ns3::TypeId GetTypeId(void);
	WifiNode();
	virtual ~WifiNode();
protected:
	virtual void DoInitialize();
	virtual void NotifyConstructionCompleted();
};

#endif /* WIFONODE_H_ */
