#include "JunctionManager.h"
#include "Junction.h"
#include "StreetManager.h"
#include "Street.h"
#include "RoadMapManager.h"
#include <TraCIScenarioManager.h>
#include <TraCICommandInterface.h>
#include <cxmlelement.h>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <iostream>

using std::string;
using std::list;
using std::vector;
using std::set;
using std::cerr;
using std::endl;
using Veins::TraCIScenarioManager;

Define_Module(RoadMapManager)


Junction *RoadMapManager::getJunctionBetweenStreets(const string & senderRoadId, const string & receiverRoadId)
{
    string sid = senderRoadId;
    string rid = receiverRoadId;

    vector<string> sendersStreetJunctionIds;
    vector<string> receiversStreetJunctionIds;

    for (uint i = 0; i < streetManager->getStreets()->size(); ++i) {
        Street* s = (*(streetManager->getStreets()))[i];
        if (!strcmp(s->getId().c_str(), sid.c_str())) {
            sendersStreetJunctionIds.push_back(s->getFromJunction()->getId());
            sendersStreetJunctionIds.push_back(s->getToJunction()->getId());
        }
        else if (!strcmp(s->getId().c_str(), rid.c_str())) {
            receiversStreetJunctionIds.push_back(s->getFromJunction()->getId());
            receiversStreetJunctionIds.push_back(s->getToJunction()->getId());
        }
    }

    // did we get the junctions?
    if ((sendersStreetJunctionIds.size() != 2) && (receiversStreetJunctionIds.size() != 2))
        error("[ERROR] could not get the needed junction between streets");

    // find the common junction
    Junction* commonJunction = NULL;

    for (uint i = 0; i < sendersStreetJunctionIds.size(); ++i) {
        JunctionMap* map = junctionManager->getJunctionMap();
        Junction* sj = (*map)[sendersStreetJunctionIds[i]];
        for (uint j = 0; j < receiversStreetJunctionIds.size(); ++j) {
            Junction* rj = (*map)[receiversStreetJunctionIds[i]];
            if (sj->getId() == rj->getId())
                commonJunction = sj;
        }
    }
//    if (!commonJunction)
//        error("[ERROR] could not get a common junction for streets %s and %s", senderRoadId.c_str(), receiverRoadId.c_str());

    return commonJunction;
}

void RoadMapManager::initialize(int stage)
{
    if (stage == 1) {
        networkXmlFile = par("networkXmlFile").xmlValue();
        junctionManager = new JunctionManager;
        streetManager = new StreetManager;

        cModule* mod = simulation.getSystemModule()->getSubmodule("manager");
        if (!mod)
            error("[ERROR] could not get the \"manager\" module");

        scenarioManager = check_and_cast<TraCIScenarioManager*>(mod);

        streetsAreLoaded = false;

        loadJunctions();

//        loadStreets();
    }
}

void RoadMapManager::finish()
{
    delete junctionManager;
    delete streetManager;
}

void RoadMapManager::loadJunctions()
{
    cXMLElementList junctionEls = networkXmlFile->getChildrenByTagName("junction");
    for (uint i = 0; i < junctionEls.size(); ++i) {
        cXMLElement* el = junctionEls[i];
        junctionManager->addJunction(el);
    }
}

void RoadMapManager::loadNetXmlFile()
{
    // since the sumo api does not provide needed values.. improvisation is required

    // get the "start" coords of the shape from the xml file..
    // find a "start" coord of the shape from the traci server.
    list<string> laneIdList = scenarioManager->getCommandInterface()->getLaneIds();
//    set<string> edgeIdSet;
//    for (list::iterator i = laneIds.begin(); i != laneIds.end(); ++i) {
//        string laneId = *i;
//        edgeIdSet.insert(scenarioManager->getCommandInterface()->getLaneEdgeId(laneId));
//    }
    cXMLElementList edgeEls = networkXmlFile->getElementsByTagName("edge");
    for (uint i = 0; i < edgeEls.size(); ++i) {
        cXMLElement* edgeEl = edgeEls[i];
        cXMLAttributeMap attrMap = edgeEl->getAttributes();
        string xmlShape = attrMap["shape"];

        Coord startCoord = getStartCoordFromShapeString(xmlShape);

        Veins::TraCICommandInterface* ci = scenarioManager->getCommandInterface();

        // now that we have a start coord from the xml file.. find a match from traci
        for (list<string>::iterator i = laneIdList.begin(); i != laneIdList.end(); ++i) {
            string laneId = *i;
            string laneEdgeId = ci->getLaneEdgeId(laneId);
            list<Veins::TraCICoord> laneShapeList = ci->getLaneShape(laneId);
            Coord coord = scenarioManager->traci2omnet(*(laneShapeList.begin()));
            if (coord.distance(startCoord) < 2) {
                // this is a match
                Street* s = (*(streetManager->getStreetMap()))[laneEdgeId];
                s->setXmlId(attrMap["id"]);
                s->setXmlShape(xmlShape);
                s->setXmlType(attrMap["type"]);
                s->setFromJunctionId(attrMap["from"]);
                s->setToJunctionId(attrMap["to"]);
                Lane* l = (*(s->getLaneMap()))[laneId];
                cXMLElementList laneEls = edgeEl->getChildrenByTagName("lane");
                for (uint i = 0; i < laneEls.size(); ++i) {
                    cXMLElement* laneEl = laneEls[i];
                    cXMLAttributeMap laneAttrMap = laneEl->getAttributes();
                    Coord laneStartCoord = getStartCoordFromShapeString(laneAttrMap["shape"]);
                    if (coord.distance(laneStartCoord) < 2) {
                        // we have a match
                        l->xmlId = laneAttrMap["id"];
                        l->xmlShape = laneAttrMap["shape"];
                        l->xmlLength = laneAttrMap["length"];
                    }
                }
            }

        }

    }
}

Coord RoadMapManager::getStartCoordFromShapeString(const string &shapeStr)
{
    char* tok;
    tok = strtok((char*)shapeStr.c_str(), " ");

    vector<string> coordStrs;

    while (tok != NULL) {
        coordStrs.push_back(string(tok));
        tok = strtok(NULL, " ");
    }

    vector<string> coordStrFields;
    string startCoordX;
    string startCoordY;

    for (uint i = 0; i < coordStrs.size(); ++i) {
        string coordStr = coordStrs[i];
        tok = strtok((char*)coordStr.c_str(), ",");

        while (tok != NULL) {
            coordStrFields.push_back(tok);
            tok = strtok(NULL, ",");
        }
    }

    for (uint i = 0; i < coordStrFields.size(); ++i) {
        startCoordX = coordStrFields[0];
        startCoordY = coordStrFields[1];
    }

    return Coord(atof(startCoordX.c_str()), atof(startCoordY.c_str()));
}


void RoadMapManager::loadStreets()
{
    if (streetsAreLoaded)
        return;

    Veins::TraCICommandInterface* ci = scenarioManager->getCommandInterface();

    list<string> traciStreetIds = ci->getEdgeIds();

    for (list<string>::iterator i = traciStreetIds.begin(); i != traciStreetIds.end(); ++i) {
        string traciId = *i;
        //cerr << "[DEBUG] traciId: " << traciId << endl;

        streetManager->addStreet(traciId);
    }

    list<string> traciLaneIds = ci->getLaneIds();

    for (list<string>::iterator i = traciLaneIds.begin(); i != traciLaneIds.end(); ++i) {

        string laneId = *i;

        string laneEdgeId = ci->getLaneEdgeId(laneId);

        streetManager->addLane(laneEdgeId, laneId);

    }

//    loadNetXmlFile();
    streetsAreLoaded = true;

}















