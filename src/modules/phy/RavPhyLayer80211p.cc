#include "RavPhyLayer80211p.h"
#include <RavObstacleControl.h>
#include <RavObstacleModel.h>
#include <iostream>
using std::cerr;
using std::endl;

Define_Module(RavPhyLayer80211p)



void RavPhyLayer80211p::initialize(int stage)
{
    //cerr << "[DEBUG] In RavPhyLayer80211p::initialize(int stage)" << endl;
    PhyLayer80211p::initialize(stage);
}

AnalogueModel* RavPhyLayer80211p::initializeRavObstacleModel(BasePhyLayer::ParameterMap & params)
{
    double carrierFrequency = 5.890e+9;
    bool useTorus = world->useTorus();
    const Coord & playgroundSize = *(world->getPgs());

    ParameterMap::iterator it;

    it = params.find("carrierFrequency");

    if (it != params.end()) {

        carrierFrequency = it->second.doubleValue();
        coreEV << "initializeRavObstacleModel(): carrierFrequency set from config.xml to " << carrierFrequency << endl;

        if(cc->hasPar("carrierFrequency") && carrierFrequency < cc->par("carrierFrequency").doubleValue())
            opp_error("TestPhyLayer::createPathLossModel(): carrierFrequency can't be smaller than specified in \
                      ConnectionManager. Please adjust your config.xml file accordingly");
    }
    else {
        if (cc->hasPar("carrierFrequency")) {

            carrierFrequency = cc->par("carrierFrequency").doubleValue();
            coreEV << "createPathLossModel(): carrierFrequency set from ConnectionManager to " << carrierFrequency << endl;
        }
        else
            coreEV << "createPathLossModel(): carrierFrequency set from default value to " << carrierFrequency << endl;
    }

    //RavObstacleControl* roc = RavObstacleControlAccess().getIfExists();
    cModule* mod = simulation.getSystemModule()->getSubmodule("obstacles");
    if (!mod)
        opp_error("[ERROR] mod is NULL");
    RavObstacleControl* roc = check_and_cast<RavObstacleControl*>(mod);


    if (!roc)
        opp_error("initializeRavObstacleModel(): cannot find RavObstacleControl module");
    RavObstacleModel* rom =  new RavObstacleModel(*roc, carrierFrequency, useTorus, playgroundSize, coreDebug);
    if (!rom)
        cerr << "[ERROR] RavObstacleModel pointer is NULL" << endl;
    return rom;
}

AnalogueModel *RavPhyLayer80211p::getAnalogueModelFromName(std::string name, BasePhyLayer::ParameterMap &params)
{
    if (name == "RavObstacleModel") {
        //cerr << "[DEBUG] returning initalizeRavObstacleModel(params)" << endl;
        return initializeRavObstacleModel(params);
    }

    return PhyLayer80211p::getAnalogueModelFromName(name, params);
}
