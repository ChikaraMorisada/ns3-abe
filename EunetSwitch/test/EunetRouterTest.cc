#include <iostream>
#include <fstream>
#define NS3_LOG_ENABLE 1
#include "ns3/log.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/bridge-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/point-to-point-helper.h"
#include "EunetSwitches.h"
#include "EunetTerminals.h"
#include "SimpleAp.h"
#include "EunetRouter.h"
//using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("EunetRouterTest");

class EunetRouterTestCase: public ns3::TestCase {
public:
	EunetRouterTestCase() :
		ns3::TestCase("EunetRouter") {
	}
	virtual ~EunetRouterTestCase() {
	}

private:
	virtual void DoRun(void);
};

void EunetRouterTestCase::DoRun() {
	//auto eunet_router_2 = ns3::CreateObject<EunetRouter>();

	//ns3::PacketMetadata::Enable();
	ns3::ObjectFactory object_factory;
	object_factory.SetTypeId("EunetRouter");
	ns3::Ptr<EunetRouter> eunet_router(object_factory.Create<EunetRouter> ());
	NS_LOG_INFO("assign eunet routers IP addresses");
	eunet_router->getTerminals().assignAddresses();
	eunet_router->getTerminals().setRemoteOfAtoB(0, 0);
	eunet_router->getTerminals().setRemoteOfAtoB(1, 0);
	eunet_router->getTerminals().Get(1)->startAt(ns3::Seconds(0.0));
	NS_LOG_INFO("Run Simulation.");
	//ns3::Simulator::Stop(ns3::Seconds(0.1));
	//ns3::Simulator::Run();
	//ns3::Simulator::Destroy();
	NS_LOG_INFO("Done.");
	eunet_router->getTerminals().logTotalRx();
}

class EunetRouterTestSuite: public ns3::TestSuite {
public:
	EunetRouterTestSuite() :
		ns3::TestSuite("EunetRouter", UNIT) {
		AddTestCase(new EunetRouterTestCase, ns3::TestCase::QUICK);
	}
};

static EunetRouterTestSuite eunet_router_test_suite;

