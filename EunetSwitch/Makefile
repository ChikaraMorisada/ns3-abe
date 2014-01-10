vpath %.h container:model:eunet
vpath %.cc container:model:eunet:test
.PHONY: test visualize

DCEDIR=$(shell find . -maxdepth 1 -type d -name dce | grep ./dce)
$(info DCEDIR=$(DCEDIR))
ifneq ($(DCEDIR),)
    VER:=DCE
    DCE:=DCE
    vpath %.so dce/build/lib:libs
    $(info DCE directory was detected)
else
    VER:=STD
    STD:=STD
    vpath %.so ../../build:libs
    $(info DCE directory was not detedted)
endif

ifeq ($(VER),)
    $(error Can\'t determine ns3 version)
endif

ifeq ($(OS),Windows_NT)
    CYGWIN:=CYGWIN
    SOSUFFIX:=dll.a
    SOPREFIX:=libns3.17-
    EXECENV:=PATH="$(PATH):../../build"
else
    LINUX:=LINUX
    SOSUFFIX:=so
    EXECENV:=LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) DCE_PATH=$(DCE_PATH) DCE_ROOT=$(DCE_ROOT) 
endif

CXXFLAGS_COMMON:= -Icontainer -Imodel -Iinclude -Ieunet -L. -Llibs -g -pg -O0 -std=c++0x -Wall -Werror
CXXFLAGS_DCE := -I./dce/build/include/ns3-dev -I./dce/build/include -L./dce/build -L./dce/build/lib -L./dce/build/include/ns3 
CXXFLAGS_STD := -I../../build -L../../build 
CXXFLAGS:=$(CXXFLAGS_COMMON) $(CXXFLAGS_$(VER))

TEST_RUNNER_STD:=../../utils/test-runner.cc
TEST_RUNNER_DCE:=dce/source/ns-3-dce/utils/test-runner.cc

PWD:=$(shell pwd)
$(info PWD=$(PWD))

ifdef DCE
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(PWD)/dce/source/ns-3-dce/build/lib
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(PWD)/dce/source/ns-3-dce/build/bin
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(PWD)/dce/source/ns-3-dce/build/bin_dce
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(PWD)/dce/build/lib
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(PWD)/dce/build/sbin
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(PWD)/dce/build/bin_dce
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):$(PWD)/dce/build/bin
    $(info LD_LIBRARY_PATH=$(LD_LIBRARY_PATH))
    DCE_PATH:=$(DCE_PATH):$(PWD)/dce/source/ns-3-dce/build/lib
    DCE_PATH:=$(DCE_PATH):$(PWD)/dce/source/ns-3-dce/build/bin
    DCE_PATH:=$(DCE_PATH):$(PWD)/dce/source/ns-3-dce/build/bin_dce
    DCE_PATH:=$(DCE_PATH):$(PWD)/dce/build/lib
    DCE_PATH:=$(DCE_PATH):$(PWD)/dce/biuld/sbin
    DCE_PATH:=$(DCE_PATH):$(PWD)/dce/build/bin_dce
    DCE_PATH:=$(DCE_PATH):$(PWD)/dce/build/bin
    $(info DCE_PATH=$(DCE_PATH))
    DCE_ROOT:=$(PWD)/dce/source/ns-3-dce/build:$(PWD)/dce/build
    $(info DCE_ROOT=$(DCE_ROOT))
    PYTHONPATH:=
endif

ifdef STD
    LD_LIBRARY_PATH:=$(LD_LIBRARY_PATH):../../build
    DCE_PATH:=
    DCE_ROOT:=
    PYTHONPATH:=../../build/bindings/python:../../src/visualizer:../../../pybindgen-0.16.0.825 
endif

#NS_LOG_STD=WifiBase=*:EunetMobile=*:CsmaInternetNode=*:CsmaNode=*:EunetMobileTest=*
NS_LOG_STD=WifiTest=*:WifiPhyTrace=*:WifiMacTrace=*
NS_LOG_DCE=DceQuaggaOspfdTest=**:DceManagerHelper=**:EunetRouterTest=**:SimpleRouter=**
#define NS_LOG
#NS_LOG="NamedRouters=**:EunetRouters=**:EunetRouter=**:SimpleRouter=**:BridgingAp=**:SimpleAp=**:NamedSwitches=**:EunetSwitch=**:EunetSwitches=**:EunetTerminal=**:EunetTerminals=**:CsmaNode=debug:OnOffApplication=info:CsmaChannelNode=debug:CsmaInternetNode=**:PacketSinkNode=debug:OnOffNode=**:SimpleSwitch=**"
#endef


EXECENV:=LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) DCE_PATH=$(DCE_PATH) DCE_ROOT=$(DCE_ROOT) PYTHONPATH=$(PYTHONPATH) NS_LOG=$(NS_LOG_$(VER))
$(info EXECENV=$(EXECENV))

SOPREFIX317:=libns3.17-
SOPREFIXDEV:=libns3-dev-

#keep alphabetical order
NS3LIBS_STD=\
    $(SOPREFIX317)antenna-debug.$(SOSUFFIX) \
    $(SOPREFIX317)aodv-debug.$(SOSUFFIX) \
    $(SOPREFIX317)applications-debug.$(SOSUFFIX) \
    $(SOPREFIX317)bridge-debug.$(SOSUFFIX) \
    $(SOPREFIX317)buildings-debug.$(SOSUFFIX) \
    $(SOPREFIX317)config-store-debug.$(SOSUFFIX) \
    $(SOPREFIX317)core-debug.$(SOSUFFIX) \
    $(SOPREFIX317)csma-debug.$(SOSUFFIX) \
    $(SOPREFIX317)csma-layout-debug.$(SOSUFFIX) \
    $(SOPREFIX317)dsdv-debug.$(SOSUFFIX) \
    $(SOPREFIX317)dsr-debug.$(SOSUFFIX) \
    $(SOPREFIX317)energy-debug.$(SOSUFFIX) \
    $(SOPREFIX317)flow-monitor-debug.$(SOSUFFIX) \
    $(SOPREFIX317)internet-debug.$(SOSUFFIX) \
    $(SOPREFIX317)lte-debug.$(SOSUFFIX) \
    $(SOPREFIX317)mesh-debug.$(SOSUFFIX) \
    $(SOPREFIX317)mpi-debug.$(SOSUFFIX) \
    $(SOPREFIX317)netanim-debug.$(SOSUFFIX) \
    $(SOPREFIX317)network-debug.$(SOSUFFIX) \
    $(SOPREFIX317)point-to-point-debug.$(SOSUFFIX) \
    $(SOPREFIX317)stats-debug.$(SOSUFFIX) \
    $(SOPREFIX317)olsr-debug.$(SOSUFFIX) \
    $(SOPREFIX317)propagation-debug.$(SOSUFFIX) \
    $(SOPREFIX317)tools-debug.$(SOSUFFIX) \
    $(SOPREFIX317)topology-read-debug.$(SOSUFFIX) \
    $(SOPREFIX317)wifi-debug.$(SOSUFFIX) \
    $(SOPREFIX317)mobility-debug.$(SOSUFFIX) \
    $(SOPREFIX317)spectrum-debug.$(SOSUFFIX) \
    $(SOPREFIX317)test-debug.$(SOSUFFIX) \
    $(SOPREFIX317)uan-debug.$(SOSUFFIX) \
    $(SOPREFIX317)virtual-net-device-debug.$(SOSUFFIX) \
    $(SOPREFIX317)visualizer-debug.$(SOSUFFIX) \
    $(SOPREFIX317)wimax-debug.$(SOSUFFIX) \
    $(SOPREFIX317)test-debug.$(SOSUFFIX) \
    $(SOPREFIX317)tools-test-debug.$(SOSUFFIX) \

# keep alphabetical order
SOPREFIX=$(SOPREFIXDEV)
NS3LIBS_DCE=\
    $(SOPREFIXDEV)antenna-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)aodv-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)applications-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)bridge-debug.so \
    $(SOPREFIXDEV)buildings-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)config-store-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)core-debug.so \
    $(SOPREFIXDEV)csma-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)csma-layout-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)dsdv-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)dsr-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)energy-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)flow-monitor-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)internet-debug.so \
    $(SOPREFIXDEV)lte-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)mesh-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)mobility-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)mpi-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)netanim-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)network-debug.so \
    $(SOPREFIXDEV)olsr-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)point-to-point-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)propagation-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)sixlowpan-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)spectrum-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)stats-debug.so \
    $(SOPREFIXDEV)topology-read-debug.so \
    $(SOPREFIXDEV)uan-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)virtual-net-device-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)wave-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)wifi-debug.$(SOSUFFIX) \
    $(SOPREFIXDEV)wimax-debug.$(SOSUFFIX) \
    libns3-dce.so libns3-dce-quagga.so \
    libns3-netlink.so \
    #libmylib.so

TESTS_SO_STD:= \
    libns3.17-core-test-debug.so \
    libns3.17-topology-read-test-debug.so \
    libns3.17-aodv-test-debug.so \
    libns3.17-applications-test-debug.so \
    libns3.17-mesh-test-debug.so \
    libns3.17-wifi-test-debug.so \
    libns3.17-uan-test-debug.so \
    libns3.17-mobility-test-debug.so \
    libns3.17-netanim-test-debug.so \
    libns3.17-internet-test-debug.so \
    libns3.17-antenna-test-debug.so \
    libns3.17-propagation-test-debug.so \
    libns3.17-network-test-debug.so \
    libns3.17-test-test-debug.so \
    libns3.17-tools-test-debug.so \
    libns3.17-stats-test-debug.so \
    libns3.17-dsr-test-debug.so \
    libns3.17-test-debug.so \
    libns3.17-wimax-test-debug.so \
    libns3.17-spectrum-test-debug.so \
    libns3.17-energy-test-debug.so \
    libns3.17-flow-monitor-test-debug.so \
    libns3.17-dsdv-test-debug.so \
    libns3.17-buildings-test-debug.so \
    libns3.17-lte-test-debug.so \
    libns3.17-olsr-test-debug.so \
    libns3.17-point-to-point-test-debug.so \


TESTS_SO_DCE:= \
    libns3-dev-mobility-test-debug.so \
    libns3-dce-test-netlink.so \
    libns3-dev-test-debug.so \
    libns3-dev-dsdv-test-debug.so \
    libns3-dev-dsr-test-debug.so \
    libns3-dev-aodv-test-debug.so \
    libns3-dev-buildings-test-debug.so \
    libns3-dev-applications-test-debug.so \
    libns3-dev-point-to-point-test-debug.so \
    libns3-dev-spectrum-test-debug.so \
    libns3-dev-stats-test-debug.so \
    libns3-dev-wave-test-debug.so \
    libtest.so \
    libns3-dce-test.so \
    libns3-dev-core-test-debug.so \
    libns3-dev-wimax-test-debug.so \
    libns3-dev-mesh-test-debug.so \
    libns3-dev-antenna-test-debug.so \
    libns3-dev-topology-read-test-debug.so \
    libns3-dev-test-test-debug.so \
    libns3-dev-energy-test-debug.so \
    libns3-dev-olsr-test-debug.so \
    libns3-dev-wifi-test-debug.so \
    libns3-dev-sixlowpan-test-debug.so \
    libns3-dev-internet-test-debug.so \
    libns3-dev-uan-test-debug.so \
    libns3-dev-netanim-test-debug.so \
    libns3-dev-flow-monitor-test-debug.so \
    libns3-dev-lte-test-debug.so \
    libns3-dev-network-test-debug.so \
    libns3-dev-propagation-test-debug.so \
    libns3-dce-quagga-test.so \


TESTS_COMMON= \
    EunetTerminalTest.o \
    EunetTerminalsTest.o \
    EunetSwitchesTest.o \
    EunetSwitchTest.o 

TESTS_STD= \
    WifiTest.o \
    #EunetMobileTest.o \
    #SimpleApTest.o \

TESTS_DCE= \
    DceQuaggaOspfdTest.o \
    EunetRouterTest.o 
    
OBJS_COMMON:= \
    NamedSwitches.o EunetSwitch.o EunetSwitches.o \
    EunetTerminal.o EunetTerminals.o CsmaNode.o CsmaChannelNode.o \
    CsmaInternetNode.o PacketSinkNode.o OnOffNode.o SimpleSwitch.o 

OBJS_STD:= \
    SimpleAp.o BridgingAp.o \
    WifiBase.o MobilityBase.o EunetMobile.o \
    TraceBase.o WifiMacTrace.o WifiPhyTrace.o

OBJS_DCE:= \
    NamedRouters.o EunetRouter.o SimpleRouter.o EunetRouters.o

libeunet.a : $(OBJS_COMMON) $(OBJS_$(VER))
	ar r $@ $^

        
build-visualize: EunetSwitchTest.o libeunet.a $(NS3LIBS) visualize.cc
	g++ -o $@ $(CXXFLAGS) $^

run-visualize:
	$(EXECENV) ./visualize --SimulatorImplementationType=ns3::VisualSimulatorImpl

gmon.out: EunetSwitchTest
	$(EXECENV) ./EunetSwitchTest

gprof: gmon.out
	$(EXECENV) gprof ./EunetSwitchTest -p -q

gdb: test-runner
	$(EXECENV) gdb ./test-runner 

doxygen:
	doxygen
	make -C latex

clean:
	-rm -f gmon.out
	-rm -f EunetSwitchTest
	-rm -f test-so test-runner
	-rm -f *.pcap *.tr
	-rm -f container/*.o model/*.o test/*.o *.o
	-rm -f *.a

test-runner: $(TEST_RUNNER_$(VER)) $(NS3LIBS_$(VER)) $(TESTS_$(VER)) libeunet.a 
	g++ -o $@ $(CXXFLAGS) $^

test-so: $(TEST_RUNNER_$(VER)) $(NS3LIBS_$(VER)) $(TESTS_SO_$(VER))
	g++ -o $@ $(CXXFLAGS) $^

gdb-test-so: test-so
	$(EXECENV) gdb ./test-so 

run-test-so: test-so
	#$(EXECENV) ./test-so --stop-on-failure --verbose
	$(EXECENV) ./test-so --verbose

test: test-runner
	$(EXECENV) ./test-runner
	#$(EXECENV) ./test-runner --stop-on-failure
	#$(EXECENV) $(NS_LOG) ./test-runner --verbose --xml --tempdir=testpy-output/2014-01-06-04-35-10-CUT --out=process-manager.xml

#build this target first to use DCE feature
install-dce-quagga: build-dce-quagga test-dce-quagga copy-shared-library copy-include-files

build-dce-quagga:
	sh build-dce-quagga.sh

run-dce-quagga:
	#$(EXECENV) dce/build/bin/test-runner --test-name=process-manager --stop-on-failure --fullness=QUICK --verbose
	#(cd dce/source/ns-3-dce; ./test.py -s process-manager --verbose)
	$(EXECENV) dce/build/bin/test-runner --verbose
	$(EXECENV) dce/build/bin/dce-quagga-ospfd 

test-dce-quagga:
	(cd dce/source/ns-3-dev; ./test.py)
	-(cd dce/source/ns-3-dce; ./test.py)

#copy-from-dce-build: copy-shared-library copy-include-files

#copy-shared-library:
#	-mkdir libs/
#	cp dce/build/lib/*.so libs/

#copy-include-files:
#	-mkdir -p include/ns3
#	cp dce/build/include/ns3/*.h include/ns3/
#	cp dce/build/include/ns3-dev/ns3/*.h include/ns3/

