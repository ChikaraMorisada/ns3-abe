#include "ns3/log.h"
#include "difs-wifi-mac.h"
NS_LOG_COMPONENT_DEFINE("DifsWifiMac");
NS_OBJECT_ENSURE_REGISTERED(DifsWifiMac);

DifsWifiMac::DifsWifiMac(uint32_t aifsn, uint32_t cwmin, uint32_t cwmax) ://default value is for 802.11b standard
	AdhocWifiMac(), aifsn(aifsn), cwmin(cwmin), cwmax(cwmax) {
	NS_LOG_FUNCTION(this);
	pAdhocWifiMac = this;
	pRegularWifiMac = this;
	pWifiMac = this;
}//constructor

DifsWifiMac::~DifsWifiMac() {
	NS_LOG_FUNCTION(this);
}//destructor

ns3::TypeId DifsWifiMac::GetTypeId(void) {
	static ns3::TypeId type_id = ns3::TypeId("DifsWifiMac").SetParent<
			AdhocWifiMac> ().AddConstructor<DifsWifiMac> ();
	return type_id;
}//GetTypeId

void DifsWifiMac::FinishConfigureStandard(enum ns3::WifiPhyStandard standard) {
	ns3::AdhocWifiMac::FinishConfigureStandard(standard);
	ns3::AdhocWifiMac::ConfigureDcf(ns3::AdhocWifiMac::m_dca, cwmin, cwmax,
			ns3::AC_BE_NQOS);
	ns3::AdhocWifiMac::m_dca->SetAifsn(aifsn);
}//FinishConfigureStandard