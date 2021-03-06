#define NS3_LOG_ENABLE 1
#include "ns3/log.h"
NS_LOG_COMPONENT_DEFINE("EunetMobile");
#define NS3_ASSERT_ENABLE 1
#include "ns3/assert.h"
#include "ns3/wifi-helper.h"
#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mobility-helper.h"
#include "EunetMobile.h"
NS_OBJECT_ENSURE_REGISTERED(EunetMobile);

ns3::TypeId EunetMobile::GetTypeId(void) {
	static ns3::TypeId type_id =
			ns3::TypeId("EunetMobile").SetParent<Base> ().AddConstructor<
					EunetMobile> ();
	return type_id;
}//GetTypeId

EunetMobile::EunetMobile() :
	TraceBase(this), MobilityBase(this, ns3::Vector(0, /*this->GetId()*/0, 0)),
			WifiBase(this, WifiBase::StaWifiMac, true) {
}// the default constructor

void EunetMobile::DoInitialize() {
	MobilityBase::DoInitialize();
	WifiBase::DoInitialize();
	Base::DoInitialize();
}

void EunetMobile::NotifyConstructionCompleted() {
	for (unsigned i = 0; i < this->GetNDevices(); ++i) {
		NS_LOG_INFO(this->GetDevice(i)->GetTypeId().GetName());
	}//for
	NS_ASSERT(this->GetNDevices() == 2);
	Base::NotifyConstructionCompleted();
	NS_ASSERT(this->GetNDevices() == 3);
}

EunetMobile::~EunetMobile() {
	// TODO !CodeTemplates.destructorstub.tododesc!
}

