#
# OMNeT++/OMNEST Makefile for francisco
#
# This file was generated with the command:
#  opp_makemake -f --deep -o francisco -O /home/prodatalab/francisco2/out -I/home/prodatalab/src/_git/veins/src/base/modules -I/home/prodatalab/src/_git/veins/src/modules/application/traci -I/home/prodatalab/src/_git/veins/src/modules/obstacle -I/home/prodatalab/src/_git/veins/src/base/messages -I/home/prodatalab/src/_git/veins/src/modules/mac/ieee80211p -I/home/prodatalab/src/_git/veins/src/modules/world/annotations -I/home/prodatalab/src/_git/veins/src/base/utils -I/home/prodatalab/src/_git/veins/src/modules/utility -I/home/prodatalab/src/_git/veins/src/base/phyLayer -I/home/prodatalab/src/_git/veins/src/modules/messages -I/home/prodatalab/src/_git/veins/src/inet -I/home/prodatalab/src/_git/veins/src/base/connectionManager -I/home/prodatalab/src/_git/veins/src/modules/analogueModel -I/home/prodatalab/src/_git/veins/src/modules/phy -I/home/prodatalab/src/_git/veins/src/modules/application/ieee80211p -I/home/prodatalab/src/_git/veins/src -I/home/prodatalab/src/_git/veins/src/modules/mobility/traci -L/home/prodatalab/src/_git/veins/out/gcc-debug/src -lveins
#

# Name of target to be created (-o option)
TARGET = francisco$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/home/prodatalab/src/_git/veins/src/base/modules \
    -I/home/prodatalab/src/_git/veins/src/modules/application/traci \
    -I/home/prodatalab/src/_git/veins/src/modules/obstacle \
    -I/home/prodatalab/src/_git/veins/src/base/messages \
    -I/home/prodatalab/src/_git/veins/src/modules/mac/ieee80211p \
    -I/home/prodatalab/src/_git/veins/src/modules/world/annotations \
    -I/home/prodatalab/src/_git/veins/src/base/utils \
    -I/home/prodatalab/src/_git/veins/src/modules/utility \
    -I/home/prodatalab/src/_git/veins/src/base/phyLayer \
    -I/home/prodatalab/src/_git/veins/src/modules/messages \
    -I/home/prodatalab/src/_git/veins/src/inet \
    -I/home/prodatalab/src/_git/veins/src/base/connectionManager \
    -I/home/prodatalab/src/_git/veins/src/modules/analogueModel \
    -I/home/prodatalab/src/_git/veins/src/modules/phy \
    -I/home/prodatalab/src/_git/veins/src/modules/application/ieee80211p \
    -I/home/prodatalab/src/_git/veins/src \
    -I/home/prodatalab/src/_git/veins/src/modules/mobility/traci \
    -I. \
    -Iresults \
    -Isrc \
    -Isrc/modules \
    -Isrc/modules/analogue \
    -Isrc/modules/application \
    -Isrc/modules/application/distance \
    -Isrc/modules/application/flooding \
    -Isrc/modules/junctions \
    -Isrc/modules/nic \
    -Isrc/modules/nodes \
    -Isrc/modules/obstacles \
    -Isrc/modules/phy \
    -Isrc/modules/roadmaps \
    -Isrc/modules/scenario \
    -Isrc/modules/stats \
    -Isrc/modules/streets \
    -Isrc/scripts

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L/home/prodatalab/src/_git/veins/out/gcc-debug/src  -lveins
LIBS += -Wl,-rpath,`abspath /home/prodatalab/src/_git/veins/out/gcc-debug/src`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/src/modules/analogue/RavObstacleModel.o \
    $O/src/modules/application/distance/DistanceApplLayer.o \
    $O/src/modules/application/flooding/FloodingApplLayer.o \
    $O/src/modules/junctions/Junction.o \
    $O/src/modules/obstacles/RavObstacleControl.o \
    $O/src/modules/phy/RavPhyLayer80211p.o \
    $O/src/modules/roadmaps/RoadMapManager.o \
    $O/src/modules/stats/FranciscoStatistics.o \
    $O/src/modules/streets/Street.o

# Message files
MSGFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f francisco francisco.exe libfrancisco.so libfrancisco.a libfrancisco.dll libfrancisco.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f results/*_m.cc results/*_m.h
	$(Q)-rm -f src/*_m.cc src/*_m.h
	$(Q)-rm -f src/modules/*_m.cc src/modules/*_m.h
	$(Q)-rm -f src/modules/analogue/*_m.cc src/modules/analogue/*_m.h
	$(Q)-rm -f src/modules/application/*_m.cc src/modules/application/*_m.h
	$(Q)-rm -f src/modules/application/distance/*_m.cc src/modules/application/distance/*_m.h
	$(Q)-rm -f src/modules/application/flooding/*_m.cc src/modules/application/flooding/*_m.h
	$(Q)-rm -f src/modules/junctions/*_m.cc src/modules/junctions/*_m.h
	$(Q)-rm -f src/modules/nic/*_m.cc src/modules/nic/*_m.h
	$(Q)-rm -f src/modules/nodes/*_m.cc src/modules/nodes/*_m.h
	$(Q)-rm -f src/modules/obstacles/*_m.cc src/modules/obstacles/*_m.h
	$(Q)-rm -f src/modules/phy/*_m.cc src/modules/phy/*_m.h
	$(Q)-rm -f src/modules/roadmaps/*_m.cc src/modules/roadmaps/*_m.h
	$(Q)-rm -f src/modules/scenario/*_m.cc src/modules/scenario/*_m.h
	$(Q)-rm -f src/modules/stats/*_m.cc src/modules/stats/*_m.h
	$(Q)-rm -f src/modules/streets/*_m.cc src/modules/streets/*_m.h
	$(Q)-rm -f src/scripts/*_m.cc src/scripts/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc results/*.cc src/*.cc src/modules/*.cc src/modules/analogue/*.cc src/modules/application/*.cc src/modules/application/distance/*.cc src/modules/application/flooding/*.cc src/modules/junctions/*.cc src/modules/nic/*.cc src/modules/nodes/*.cc src/modules/obstacles/*.cc src/modules/phy/*.cc src/modules/roadmaps/*.cc src/modules/scenario/*.cc src/modules/stats/*.cc src/modules/streets/*.cc src/scripts/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/modules/analogue/RavObstacleModel.o: src/modules/analogue/RavObstacleModel.cc \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Mapping.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICoord.h \
  src/modules/junctions/Junction.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  /home/prodatalab/src/_git/veins/src/modules/world/annotations/AnnotationManager.h \
  src/modules/obstacles/RavObstacleControl.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/ObstacleControl.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/BaseConnectionManager.h \
  src/modules/streets/Street.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/Obstacle.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIConnection.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIBuffer.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIColor.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/NicEntry.h \
  /home/prodatalab/src/_git/veins/src/base/utils/Move.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseWorldUtility.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/MappingUtils.h \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/MappingBase.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Signal_.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/utils/FindModule.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Interpolation.h \
  /home/prodatalab/src/_git/veins/src/base/messages/AirFrame_m.h \
  src/modules/analogue/RavObstacleModel.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManager.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/AnalogueModel.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManagerLaunchd.h
$O/src/modules/application/distance/DistanceApplLayer.o: src/modules/application/distance/DistanceApplLayer.cc \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseWorldUtility.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIConnection.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIBuffer.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/ChannelAccess.h \
  src/modules/application/distance/DistanceApplLayer.h \
  src/modules/stats/FranciscoStatistics.h \
  /home/prodatalab/src/_git/veins/src/modules/application/ieee80211p/BaseWaveApplLayer.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  /home/prodatalab/src/_git/veins/src/base/utils/SimpleAddress.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManager.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseBattery.h \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIMobility.h \
  /home/prodatalab/src/_git/veins/src/base/utils/Move.h \
  /home/prodatalab/src/_git/veins/src/modules/mac/ieee80211p/WaveAppToMac1609_4Interface.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICommandInterface.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIColor.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/NicEntry.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/BaseConnectionManager.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/ObstacleControl.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/Obstacle.h \
  /home/prodatalab/src/_git/veins/src/modules/world/annotations/AnnotationManager.h \
  /home/prodatalab/src/_git/veins/src/modules/messages/WaveShortMessage_m.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseApplLayer.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICoord.h \
  /home/prodatalab/src/_git/veins/src/base/utils/PassedMessage.h \
  /home/prodatalab/src/_git/veins/src/modules/utility/Consts80211p.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseModule.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseMobility.h \
  /home/prodatalab/src/_git/veins/src/base/utils/FindModule.h \
  /home/prodatalab/src/_git/veins/src/base/utils/NetwToMacControlInfo.h \
  /home/prodatalab/src/_git/veins/src/base/utils/HostState.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BatteryAccess.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseLayer.h
$O/src/modules/application/flooding/FloodingApplLayer.o: src/modules/application/flooding/FloodingApplLayer.cc \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICoord.h \
  /home/prodatalab/src/_git/veins/src/base/utils/PassedMessage.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseApplLayer.h \
  /home/prodatalab/src/_git/veins/src/modules/world/annotations/AnnotationManager.h \
  /home/prodatalab/src/_git/veins/src/modules/messages/WaveShortMessage_m.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/ObstacleControl.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/BaseConnectionManager.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/Obstacle.h \
  /home/prodatalab/src/_git/veins/src/modules/mac/ieee80211p/WaveAppToMac1609_4Interface.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIColor.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICommandInterface.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/NicEntry.h \
  /home/prodatalab/src/_git/veins/src/base/utils/Move.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseLayer.h \
  /home/prodatalab/src/_git/veins/src/base/utils/NetwToMacControlInfo.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BatteryAccess.h \
  /home/prodatalab/src/_git/veins/src/base/utils/HostState.h \
  /home/prodatalab/src/_git/veins/src/base/utils/FindModule.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseMobility.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseModule.h \
  /home/prodatalab/src/_git/veins/src/modules/utility/Consts80211p.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  /home/prodatalab/src/_git/veins/src/modules/application/ieee80211p/BaseWaveApplLayer.h \
  src/modules/stats/FranciscoStatistics.h \
  src/modules/application/flooding/FloodingApplLayer.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/ChannelAccess.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIConnection.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIBuffer.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseWorldUtility.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIMobility.h \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseBattery.h \
  /home/prodatalab/src/_git/veins/src/base/utils/SimpleAddress.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManager.h
$O/src/modules/junctions/Junction.o: src/modules/junctions/Junction.cc \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  src/modules/junctions/Junction.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h
$O/src/modules/obstacles/RavObstacleControl.o: src/modules/obstacles/RavObstacleControl.cc \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICoord.h \
  /home/prodatalab/src/_git/veins/src/modules/world/annotations/AnnotationManager.h \
  src/modules/obstacles/RavObstacleControl.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/Obstacle.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/BaseConnectionManager.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/ObstacleControl.h \
  src/modules/streets/Street.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/NicEntry.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIColor.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICommandInterface.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/MappingUtils.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/MappingBase.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/utils/FindModule.h \
  /home/prodatalab/src/_git/veins/src/base/messages/AirFrame_m.h \
  src/modules/junctions/Junction.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Mapping.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  src/modules/roadmaps/RoadMapManager.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIBuffer.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIConnection.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseWorldUtility.h \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Signal_.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Interpolation.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManager.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManagerLaunchd.h
$O/src/modules/phy/RavPhyLayer80211p.o: src/modules/phy/RavPhyLayer80211p.cc \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManagerLaunchd.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/AnalogueModel.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManager.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/ChannelState.h \
  /home/prodatalab/src/_git/veins/src/modules/analogueModel/LogNormalShadowing.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseBattery.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Interpolation.h \
  src/modules/analogue/RavObstacleModel.h \
  /home/prodatalab/src/_git/veins/src/modules/mac/ieee80211p/Mac80211pToPhy11pInterface.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Signal_.h \
  /home/prodatalab/src/_git/veins/src/modules/analogueModel/SimplePathlossModel.h \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/BaseDecider.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseWorldUtility.h \
  /home/prodatalab/src/_git/veins/src/base/messages/ChannelSenseRequest_m.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/MacToPhyInterface.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIBuffer.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIConnection.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/BasePhyLayer.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/ChannelAccess.h \
  src/modules/roadmaps/RoadMapManager.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/DeciderToPhyInterface.h \
  src/modules/junctions/Junction.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Mapping.h \
  /home/prodatalab/src/_git/veins/src/modules/utility/Consts80211p.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseModule.h \
  /home/prodatalab/src/_git/veins/src/modules/analogueModel/JakesFading.h \
  /home/prodatalab/src/_git/veins/src/modules/phy/PhyLayer80211p.h \
  /home/prodatalab/src/_git/veins/src/base/messages/AirFrame_m.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseMobility.h \
  /home/prodatalab/src/_git/veins/src/base/utils/FindModule.h \
  /home/prodatalab/src/_git/veins/src/base/utils/HostState.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/modules/BatteryAccess.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/MappingBase.h \
  /home/prodatalab/src/_git/veins/src/modules/phy/Decider80211p.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/ChannelInfo.h \
  /home/prodatalab/src/_git/veins/src/modules/phy/Decider80211pToPhy80211pInterface.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/MappingUtils.h \
  /home/prodatalab/src/_git/veins/src/base/utils/Move.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/NicEntry.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h \
  src/modules/phy/RavPhyLayer80211p.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIColor.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICommandInterface.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/Obstacle.h \
  src/modules/streets/Street.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/BaseConnectionManager.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/ObstacleControl.h \
  /home/prodatalab/src/_git/veins/src/modules/phy/SNRThresholdDecider.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/PhyUtils.h \
  /home/prodatalab/src/_git/veins/src/modules/world/annotations/AnnotationManager.h \
  src/modules/obstacles/RavObstacleControl.h \
  /home/prodatalab/src/_git/veins/src/base/phyLayer/Decider.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICoord.h
$O/src/modules/roadmaps/RoadMapManager.o: src/modules/roadmaps/RoadMapManager.cc \
  /home/prodatalab/src/_git/veins/src/base/modules/BaseWorldUtility.h \
  src/modules/roadmaps/RoadMapManager.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIBuffer.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIConnection.h \
  src/modules/junctions/Junction.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIScenarioManager.h \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/ObstacleControl.h \
  src/modules/streets/Street.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/BaseConnectionManager.h \
  /home/prodatalab/src/_git/veins/src/modules/obstacle/Obstacle.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICommandInterface.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCIColor.h \
  /home/prodatalab/src/_git/veins/src/base/connectionManager/NicEntry.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h \
  /home/prodatalab/src/_git/veins/src/modules/world/annotations/AnnotationManager.h \
  /home/prodatalab/src/_git/veins/src/modules/mobility/traci/TraCICoord.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/utils/FindModule.h
$O/src/modules/stats/FranciscoStatistics.o: src/modules/stats/FranciscoStatistics.cc \
  src/modules/stats/FranciscoStatistics.h
$O/src/modules/streets/Street.o: src/modules/streets/Street.cc \
  /home/prodatalab/src/_git/veins/src/inet/FWMath.h \
  /home/prodatalab/src/_git/veins/src/base/utils/miximkerneldefs.h \
  src/modules/junctions/Junction.h \
  /home/prodatalab/src/_git/veins/src/inet/Coord.h \
  /home/prodatalab/src/_git/veins/src/base/utils/MiXiMDefs.h \
  src/modules/streets/Street.h
