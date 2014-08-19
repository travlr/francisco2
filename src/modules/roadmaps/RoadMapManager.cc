#include "Junction.h"
#include "Street.h"
#include "RoadMapManager.h"
#include <TraCIScenarioManager.h>
#include <TraCICommandInterface.h>
#include <TraCICoord.h>
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
using Veins::TraCICoord;

Define_Module(RoadMapManager)


void RoadMapManager::initialize(int stage)
{
    if (stage == 1) {

        cModule* mod = simulation.getSystemModule()->getSubmodule("manager");
        if (!mod)
            error("[ERROR] could not get the \"manager\" module");

        scenarioManager = check_and_cast<TraCIScenarioManager*>(mod);

        ci = scenarioManager->getCommandInterface();

        streetsAreLoaded = junctionsAreLoaded = false;

        if (ci) {
            haveCI = true;
            loadJunctions();
            loadStreets();
        }
        else {
            haveCI = false;
            scheduleAt(simTime() + 5, new cMessage("ci"));
        }

    }
}

void RoadMapManager::handleMessage(cMessage *msg)
{
    if (!strcmp(msg->getName(), "ci")) {
        cerr << "[DEBUG] In RoadMapManager::handleMessage()" << endl;
        ci = scenarioManager->getCommandInterface();
        loadJunctions();
        loadStreets();
    }
}

void RoadMapManager::finish()
{
}

void RoadMapManager::loadJunctions()
{
    if (junctionsAreLoaded)
        return;

    junctionIds = ci->getJunctionIds();

    for (list<string>::iterator i = junctionIds.begin(); i != junctionIds.end(); ++i) {
        string id = *i;
        Coord position = scenarioManager->traci2omnet(ci->getJunctionPosition(id));
        Junction* j = new Junction(id, position);
        junctions.push_back(j);
        junctionMap[id] = j;
    }

    junctionsAreLoaded = true;
}


//Junction *RoadMapManager::getJunctionBetweenStreets(const string & senderRoadId, const string & receiverRoadId)
//{

//}



void RoadMapManager::loadStreets()
{
    if (streetsAreLoaded)
        return;

    std::list<string> laneIds = ci->getLaneIds();

    for (list<string>::iterator i = laneIds.begin(); i != laneIds.end(); ++i) {
        string laneId = *i;
        string streetId = ci->getLaneEdgeId(laneId);
        streetIdSet.insert(streetId);

        Street* s;

        // is the lanes edge id already in the street vector?
        bool alreadyHaveStreet = false;
        for (uint i = 0; i < streets.size(); ++i) {
            s = streets[i];
            if (!strcmp(s->getId().c_str(), streetId.c_str())) {
                alreadyHaveStreet = true;
                break;
            }
        }

        // add new street and its lanes
        if (!alreadyHaveStreet) {
            s = new Street(streetId);
            streets.push_back(s);
            streetMap[streetId] = s;
            cerr << "[DEBUG] new street: " << streetId << endl;
            s->addLane(laneId);
            s->setLaneWidth(laneId, ci->getLaneWidth(laneId));
            s->setLaneShape(laneId, scenarioManager->traci2omnet(ci->getLaneShape(laneId)));
            //s->setLaneIndex(laneId, ci->getLaneIndex(streetId));
        }

        // get the street's junctions
        for (uint i = 0; i < junctions.size(); ++i) {

            Junction* jn = junctions[i];
            bool junctionCorrelatedWithEdge = false;

            Coord jpos = jn->getPosition();

            Coord startPosition;
            Coord endPosition;

            Lanes lanes = (*(s->getLanes()));
            Lane* l;

            if (lanes.size() == 1)
                l = lanes[0];
            else
                error("[DEBUG] there are %u lanes", lanes.size());

            startPosition = l->shapeCoords[0];
            endPosition = l->shapeCoords[l->shapeCoords.size()-1];

            if (jpos.distance(startPosition) < 10) {
                s->setFromJunction(jn);
                junctionCorrelatedWithEdge = true;
            }

            else if (jpos.distance(endPosition) < 10) {
                s->setToJunction(jn);
                junctionCorrelatedWithEdge = true;
            }

//            for (uint j = 0; j < streets.size(); ++i) {
//                Street* s = streets[j];
//                Coord startPosition;
//                Coord endPosition;
//                Lanes lanes = (*(s->getLanes()));
//                for (uint k = 0; k < lanes.size(); ++i) {
//                    Lane* l = lanes[k];
//                    if (l->index != 0)
//                        continue;
//                    startPosition = l->shapeCoords[0];
//                    endPosition = l->shapeCoords[l->shapeCoords.size()-1];
//                }

//                if (jpos.distance(startPosition) < 10) {
//                    s->setFromJunction(jn);
//                    junctionCorrelatedWithEdge = true;
//                }
//                else if (jpos.distance(endPosition) < 10) {
//                    s->setToJunction(jn);
//                    junctionCorrelatedWithEdge = true;
//                }
//            }

            if (!junctionCorrelatedWithEdge) {
                cerr << "[ERROR] junction \"" << jn->getId().c_str() << "\" was not correlated with edge" << endl;
            }
        }
    }
    streetsAreLoaded = true;
}















