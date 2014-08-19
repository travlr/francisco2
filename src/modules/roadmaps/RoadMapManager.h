#ifndef ROADMAPMANAGER_H
#define ROADMAPMANAGER_H

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <csimplemodule.h>
#include <cxmlelement.h>
#include <TraCIScenarioManager.h>
#include <TraCICommandInterface.h>
using Veins::TraCIScenarioManager;
using Veins::TraCICommandInterface;

class Junction;
class Street;


class RoadMapManager : public cSimpleModule
{

public:
    void loadJunctions();
    void loadStreets();
    std::map<std::string,Street*> & getStreetMap() { return streetMap; }
    std::map<std::string,Junction*> & getJunctionMap() { return junctionMap; }
    bool getStreetsAreLoaded() { return streetsAreLoaded; }
    bool getJunctionsAreLoaded() { return junctionsAreLoaded; }
    bool getHaveCI() { return haveCI; }
    void setupCI() { ci = scenarioManager->getCommandInterface(); if (ci) haveCI=true; else haveCI=false; }

protected:
    TraCIScenarioManager*   scenarioManager;
    TraCICommandInterface*  ci;
    bool                    streetsAreLoaded;
    bool                    junctionsAreLoaded;
    bool                    haveCI;
    std::set<std::string>   streetIdSet;
    std::list<std::string>  junctionIds;
    std::vector<Street*>    streets;
    std::vector<Junction*>  junctions;
    std::map<std::string,Street*>    streetMap;
    std::map<std::string,Junction*>  junctionMap;

protected:
    virtual void initialize(int stage);
    virtual int numInitStages() const { return 2; }
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif // ROADMAPMANAGER_H
