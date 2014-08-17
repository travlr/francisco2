#ifndef ROADMAPMANAGER_H
#define ROADMAPMANAGER_H

#include <csimplemodule.h>
#include <cxmlelement.h>
#include <TraCIScenarioManager.h>
using Veins::TraCIScenarioManager;


class JunctionManager;
class StreetManager;
class Junction;
class Street;
//namespace {
//class TraCIScenarioManager;
//}

class RoadMapManager : public cSimpleModule
{

public:
    JunctionManager*    getJunctionManager() { return junctionManager; }
    StreetManager*      getStreetManager() { return streetManager; }
    Junction *getJunctionBetweenStreets(const std::string & senderRoadId, const std::string & receiverRoadId);
    bool getStreetsAreLoaded() const { return streetsAreLoaded; }
    void loadStreets();

protected:
    cXMLElement*            networkXmlFile;
    JunctionManager*        junctionManager;
    StreetManager*          streetManager;
    TraCIScenarioManager*   scenarioManager;
    bool                    streetsAreLoaded;

protected:
    virtual void initialize(int stage);
    virtual int numInitStages() const { return 2; }
    virtual void finish();

    void loadJunctions();
    void loadNetXmlFile();
    Coord getStartCoordFromShapeString(const string & shapeStr);


};

#endif // ROADMAPMANAGER_H
