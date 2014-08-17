//
// RavObstacleControl - models obstacles that block radio transmissions
// Copyright (C) 2010 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#include <sstream>
#include <map>
#include <set>
#include <math.h>

#include <iostream>
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include <list>
using std::list;
#include <vector>
using std::vector;
#include <numeric>
#include <cstdio>
using std::sprintf;
#include <TraCIScenarioManagerLaunchd.h>
using Veins::TraCIScenarioManagerLaunchd;
using Veins::TraCIScenarioManagerLaunchdAccess;
#include <TraCICommandInterface.h>
using Veins::TraCICommandInterface;
#include <AirFrame_m.h>
using Veins::AirFrame;
#include "RavObstacleControl.h"
#include "RoadMapManager.h"
#include "Junction.h"
#include "JunctionManager.h"
#include "Street.h"
#include "StreetManager.h"

#define GRIDCELL_SIZE 1024


Define_Module(RavObstacleControl)



void RavObstacleControl::initialize(int stage) {
    if (stage == 1)	{

        //cerr << "[DEBUG] IN RavObstacleControl::initialize(int stage)" << endl;

//		debug = par("debug");

		obstacles.clear();
//		cacheEntries.clear();

		annotations = AnnotationManagerAccess().getIfExists();
		if (annotations) annotationGroup = annotations->createGroup("obstacles");

        obstaclesXml                                    = par("obstacles");
        streetAngularDifferenceThreshold                = par("streetAngularDifferenceThreshold");
        distanceFromVehicleToStreetProjectionThreshold  = par("distanceFromVehicleToStreetProjectionThreshold");
        distanceOfVehicleFromJunctionThreshold          = par("distanceOfVehicleFromJunctionThreshold");

        scenarioManager = TraCIScenarioManagerLaunchdAccess().get();

        attenuationState = new AttenuationState;

//		addFromXml(obstaclesXml);

        cModule* mod = simulation.getSystemModule()->getSubmodule("roadmapmgr");
        if (!mod)
            error("[ERROR] could not get module roadmapmgr");
        roadMapManager = check_and_cast<RoadMapManager*>(mod);
	}
}

void RavObstacleControl::finish() {
    //    for (RavObstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i) {
    //        for (RavObstacleGridRow::iterator j = i->begin(); j != i->end(); ++j) {
    //			while (j->begin() != j->end()) erase(*j->begin());
    //		}
    //	}
    //	obstacles.clear();
}

void RavObstacleControl::handleMessage(cMessage *msg) {
	if (msg->isSelfMessage()) {
		handleSelfMsg(msg);
		return;
	}
    error("RavObstacleControl doesn't handle messages from other modules");
}

void RavObstacleControl::handleSelfMsg(cMessage *msg) {
    error("RavObstacleControl doesn't handle self-messages");
}

//void RavObstacleControl::addFromXml(cXMLElement* xml) {
//	std::string rootTag = xml->getTagName();
//	ASSERT (rootTag == "obstacles");

//	cXMLElementList list = xml->getChildren();
//	for (cXMLElementList::const_iterator i = list.begin(); i != list.end(); ++i) {
//		cXMLElement* e = *i;

//		std::string tag = e->getTagName();
//		ASSERT(tag == "poly");

//		// <poly id="building#0" type="building" color="#F00" shape="16,0 8,13.8564 -8,13.8564 -16,0 -8,-13.8564 8,-13.8564" />
//		ASSERT(e->getAttribute("id"));
//		std::string id = e->getAttribute("id");
//		ASSERT(e->getAttribute("type"));
//		std::string type = e->getAttribute("type");
//		ASSERT(e->getAttribute("color"));
//		std::string color = e->getAttribute("color");
//		ASSERT(e->getAttribute("shape"));
//		std::string shape = e->getAttribute("shape");

//		double attenuationPerWall = 50; /**< in dB */
//		double attenuationPerMeter = 1; /**< in dB / m */
//		if (type == "building") { attenuationPerWall = 50; attenuationPerMeter = 1; }
//		else error("unknown obstacle type: %s", type.c_str());
//        RavObstacle obs(id, attenuationPerWall, attenuationPerMeter);
//		std::vector<Coord> sh;
//		cStringTokenizer st(shape.c_str());
//		while (st.hasMoreTokens()) {
//			std::string xy = st.nextToken();
//			std::vector<double> xya = cStringTokenizer(xy.c_str(), ",").asDoubleVector();
//			ASSERT(xya.size() == 2);
//			sh.push_back(Coord(xya[0], xya[1]));
//		}
//		obs.setShape(sh);
//		add(obs);

//	}

//}

//void RavObstacleControl::add(RavObstacle obstacle) {
//    RavObstacle* o = new RavObstacle(obstacle);

//	size_t fromRow = std::max(0, int(o->getBboxP1().x / GRIDCELL_SIZE));
//	size_t toRow = std::max(0, int(o->getBboxP2().x / GRIDCELL_SIZE));
//	size_t fromCol = std::max(0, int(o->getBboxP1().y / GRIDCELL_SIZE));
//	size_t toCol = std::max(0, int(o->getBboxP2().y / GRIDCELL_SIZE));
//	for (size_t row = fromRow; row <= toRow; ++row) {
//		for (size_t col = fromCol; col <= toCol; ++col) {
//			if (obstacles.size() < col+1) obstacles.resize(col+1);
//			if (obstacles[col].size() < row+1) obstacles[col].resize(row+1);
//			(obstacles[col])[row].push_back(o);
//		}
//	}

//	// visualize using AnnotationManager
//	if (annotations) o->visualRepresentation = annotations->drawPolygon(o->getShape(), "red", annotationGroup);

//////    cacheEntries.clear();
////}

//void RavObstacleControl::erase(const RavObstacle* obstacle) {
//    for (RavObstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i) {
//        for (RavObstacleGridRow::iterator j = i->begin(); j != i->end(); ++j) {
//            for (RavObstacleGridCell::iterator k = j->begin(); k != j->end(); ) {
//                RavObstacle* o = *k;
//				if (o == obstacle) {
//					k = j->erase(k);
//				} else {
//					++k;
//				}
//			}
//		}
//	}

//	if (annotations && obstacle->visualRepresentation) annotations->erase(obstacle->visualRepresentation);
//    delete obstacle;

////	cacheEntries.clear();
//}

double RavObstacleControl::calculateAttenuation(const Veins::AirFrame *frame, const Coord &senderPos, const Coord &receiverPos)
{
    initializeAttenuationState(frame, senderPos, receiverPos);


    bool vaiss = vehiclesHaveLineOfSight();
    bool sviij  = senderVehicleIsInJunction();
    bool vsaa   = vehicleStreetsAreAdjacent();

    if (vaiss || sviij || vsaa)
        return 1;

    return 0;
}

void RavObstacleControl::initializeAttenuationState(const Veins::AirFrame* frame, const Coord & senderPos, const Coord & receiverPos)
{
    if (attenuationState->sharedJunction)
        attenuationState->sharedJunction = NULL;

    if (!roadMapManager->getStreetsAreLoaded())
        roadMapManager->loadStreets();

    memset(attenuationState->senderIdStr, 0, strlen(attenuationState->senderIdStr));
    memset(attenuationState->receiverIdStr, 0, strlen(attenuationState->receiverIdStr));
    memset(attenuationState->senderRoadId, 0, strlen(attenuationState->senderRoadId));
    memset(attenuationState->receiverRoadId, 0, strlen(attenuationState->receiverRoadId));
    memset(attenuationState, 0, sizeof(*attenuationState));

    AttenuationState* as = attenuationState;

////    as->senderIdStr.clear();
////    as->receiverIdStr.clear();

    strcpy(as->senderIdStr, getVehicleIdFromCoord(senderPos).c_str());
    strcpy(as->receiverIdStr, getVehicleIdFromCoord(receiverPos).c_str());

    as->senderId = atof(as->senderIdStr);
    as->receiverId = atof(as->receiverIdStr);

////    cerr << "[DEBUG] as->senderId = " << as->senderId << endl;
////    cerr << "[DEBUG] as->receiverId = " << as->receiverId << endl;

////    sprintf(as->senderIdStr, "%d", as->senderId);
////    sprintf(as->receiverIdStr, "%d", as->receiverId);

////    cerr << "[DEBUG] as->senderIdStr: " << as->senderIdStr << endl;
////    cerr << "[DEBUG] as->receiverIdStr: " << as->receiverIdStr << endl;

    strcpy(as->senderRoadId, scenarioManager->getCommandInterface()->getVehicleRoadId(string(as->senderIdStr)).c_str());
    strcpy(as->receiverRoadId, scenarioManager->getCommandInterface()->getVehicleRoadId(string(as->receiverIdStr)).c_str());

    as->sharedJunction  = roadMapManager->getJunctionBetweenStreets(as->senderRoadId, as->receiverRoadId);

    // get streets
    as->senderStreet = (*(roadMapManager->getStreetManager()->getStreetMap()))[as->senderRoadId];
    as->receiverStreet = (*(roadMapManager->getStreetManager()->getStreetMap()))[as->receiverRoadId];

    // get street segments
    as->senderStreetSegments = as->senderStreet->getStreetSegments();
    as->receiverStreetSegments = as->receiverStreet->getStreetSegments();

    // get street widths
    as->senderStreetWidth   = as->senderStreet->getLaneWidth(0);
    as->receiverStreetWidth = as->receiverStreet->getLaneWidth(0);

    if (as->senderRoadId == as->receiverRoadId)
        as->areOnSameStreet = true;

    // are on adjacent streets
    if (as->sharedJunction) {
        as->areOnAdjacentStreets = true;
        as->streetAngle = getStreetAngle(as->senderIdStr, as->receiverIdStr);
    }
}

bool RavObstacleControl::vehiclesHaveLineOfSight()
{
    AttenuationState* as = attenuationState;
    StreetSegment*    ss = as->senderStreetSegment;
    double            th_s = distanceFromVehicleToStreetProjectionThreshold;

    double minDist = getMinDistanceFromPointToLine(as->receiverPosition, ss->startPosition, ss->endPosition);

    if (    (minDist <= th_s)
         && (ss->startPosition.x - th_s < as->receiverPosition.x)
         && (as->receiverPosition.x < ss->endPosition.x + th_s)
         && (ss->startPosition.y - th_s < as->receiverPosition.y)
         && (as->receiverPosition.y > ss->endPosition.y + th_s)
       ) {
        return true;
    }
    return false;
}

bool RavObstacleControl::senderVehicleIsInJunction()
{
    AttenuationState* as = attenuationState;
    string fromId = as->senderStreet->getFromJunction()->getId();
    string toId = as->senderStreet->getToJunction()->getId();
    Junction* from = (*(roadMapManager->getJunctionManager()->getJunctionMap()))[fromId];
    Junction* to = (*(roadMapManager->getJunctionManager()->getJunctionMap()))[toId];

    if (as->senderPosition.distance(*(from->getPosition())) < distanceOfVehicleFromJunctionThreshold) {
        as->junctionSenderIsIn = from;
        return true;
    }

    if (as->senderPosition.distance(*(to->getPosition())) < distanceOfVehicleFromJunctionThreshold) {
        as->junctionSenderIsIn = to;
        return true;
    }

    return false;
}



bool RavObstacleControl::vehicleStreetsAreAdjacent()
{
    AttenuationState* as = attenuationState;

    double senderYdist = as->senderStreetSegment->endPosition.y - as->senderStreetSegment->startPosition.y;
    double senderXdist = as->senderStreetSegment->endPosition.x - as->senderStreetSegment->startPosition.x;

    double receiverYdist = as->receiverStreetSegment->endPosition.y - as->receiverStreetSegment->startPosition.y;
    double receiverXdist = as->receiverStreetSegment->endPosition.x - as->receiverStreetSegment->startPosition.x;


    double anglePrime = abs(atan2(senderYdist, senderXdist) - atan2(receiverYdist, receiverXdist));

    if (anglePrime > 360 - anglePrime)
        anglePrime = 360 - anglePrime;

    if (anglePrime < streetAngularDifferenceThreshold)
        return true;

    return false;
}

//double RavObstacleControl::calculateAttenuation(const AirFrame* frame, const Coord& senderPos, const Coord& receiverPos) {

//    Enter_Method_Silent();

//    bool areOnSameStreet        = false;
//    bool areOnSameStreetSegment = false;
//    bool senderIsInJunction     = false;
//    bool receiverIsInJunction   = false;
//    bool areOnAdjacentStreets   = false;

//    double streetAngle          = 0;


//    int senderId     = frame->getSenderModule()->getId();
//    int receiverId   = frame->getArrivalModule()->getId();

//    char senderIdStr[32];
//    char receiverIdStr[32];

//    sprintf(senderIdStr, "%d", frame->getSenderModule()->getId());
//    sprintf(senderIdStr, "%d", frame->getArrivalModule()->getId());

//    string senderRoadId = scenarioManager->getCommandInterface()->getVehicleRoadId(senderIdStr);
//    string receiverRoadId = scenarioManager->getCommandInterface()->getVehicleRoadId(receiverIdStr);

//    /// NOTE: same road Id is not good enough considering the shape of a road is a polyline
//    // if they are on the same street, then line-of-site is assumed
//    if (senderRoadId == receiverRoadId) {
//        areOnSameStreet = true;
//    }
//    /// END NOTE

//    if (areOnSameStreet)
//        return 1;

//    // get junction between streets
//    Junction* j = roadMapManager->getJunctionBetweenStreets(senderRoadId, receiverRoadId);

//    if (j) {
//        // angle between streets (vehicles)
//        streetAngle = getStreetAngle(senderIdStr, receiverIdStr);
//        areOnAdjacentStreets = true;
//    }
//    // get streets
//    Street* senderStreet = (*(roadMapManager->getStreetManager()->getStreetMap()))[senderRoadId];
//    Street* receiverStreet = (*(roadMapManager->getStreetManager()->getStreetMap()))[receiverRoadId];

//    // get street widths
//    double sendersStreetWidth = senderStreet->getLaneWidth(0);
//    double receiversStreetWidth = receiverStreet->getLaneWidth(0);

//    //
//    /*different streets with line of site (OR SAME STREET.. EACH VEH ON DIFFRENT POLYLINE SEGMENTS)?
//    double cPrime =
//    d_av = senderPos.distance(*(j->getPosition()));
//    d_acPrime = d_av - (((receiversStreetWidth / 2) / sin(streetAngle)) - (((sendersStreetWidth / 2) / tan(streetAngle))))*/

//    // is line of site ==
//        // d(Bprime, Cprime) < l_A
//            // d(Bprime, Cprime) = d(A, Cprime) * d(B, C) / d(A, C)
//                // d(A, Cprime) = d(A, V) - d(Cprime, V)
//                    // d(A, V) = l_B / sin(streetAngle) - l_A / tan(streetAngle)
//                // d(B, C) = dmin(B, r) = getMinDistanceFromPointToLine(..)
//                // d(A, C) = d(A, V) + d(V, C)
//                    // d(V, C) = d(B, C) / tan(streetAngle)

//    d_VC = senderStreet->getS





////	// return cached result, if available
////	CacheKey cacheKey(senderPos, receiverPos);
////	CacheEntries::const_iterator cacheEntryIter = cacheEntries.find(cacheKey);
////	if (cacheEntryIter != cacheEntries.end()) return cacheEntryIter->second;

////	// calculate bounding box of transmission
////	Coord bboxP1 = Coord(std::min(senderPos.x, receiverPos.x), std::min(senderPos.y, receiverPos.y));
////	Coord bboxP2 = Coord(std::max(senderPos.x, receiverPos.x), std::max(senderPos.y, receiverPos.y));

////	size_t fromRow = std::max(0, int(bboxP1.x / GRIDCELL_SIZE));
////	size_t toRow = std::max(0, int(bboxP2.x / GRIDCELL_SIZE));
////	size_t fromCol = std::max(0, int(bboxP1.y / GRIDCELL_SIZE));
////	size_t toCol = std::max(0, int(bboxP2.y / GRIDCELL_SIZE));

////    std::set<RavObstacle*> processedRavObstacles;
////	double factor = 1;
////	for (size_t col = fromCol; col <= toCol; ++col) {
////		if (col >= obstacles.size()) break;
////		for (size_t row = fromRow; row <= toRow; ++row) {
////			if (row >= obstacles[col].size()) break;
////            const RavObstacleGridCell& cell = (obstacles[col])[row];
////            for (RavObstacleGridCell::const_iterator k = cell.begin(); k != cell.end(); ++k) {

////                RavObstacle* o = *k;

////                if (processedRavObstacles.find(o) != processedRavObstacles.end())
////                    continue;
////                processedRavObstacles.insert(o);

////				// bail if bounding boxes cannot overlap
////				if (o->getBboxP2().x < bboxP1.x) continue;
////				if (o->getBboxP1().x > bboxP2.x) continue;
////				if (o->getBboxP2().y < bboxP1.y) continue;
////				if (o->getBboxP1().y > bboxP2.y) continue;

////				double factorOld = factor;

////				factor *= o->calculateAttenuation(senderPos, receiverPos);

////				// draw a "hit!" bubble
////				if (annotations && (factor != factorOld)) annotations->drawBubble(o->getBboxP1(), "hit");

////				// bail if attenuation is already extremely high
////				if (factor < 1e-30) break;

////			}
////		}
////	}

////	// cache result
////	if (cacheEntries.size() >= 1000) cacheEntries.clear();
////	cacheEntries[cacheKey] = factor;

//////    if (factor != 1)
////        //cerr << " ...IS  BLOCKED" << endl;
//////    else
//////        cerr << "sender: " << senderPos << "and receiver: " << receiverPos << " ...NOT BLOCKED" << endl;

////    cerr << factor << endl;

//    //	return factor;
//}



bool RavObstacleControl::pointIsInBoundingBox(const Coord &point, const Coord &startPoint, const Coord &endPoint)
{
    if ((point.x >= startPoint.x) && (point.y >= startPoint.y) && (point.x <= endPoint.x) && (point.y <= endPoint.y))
        return true;
    return false;
}

//pair<Coord,Coord> RavObstacleControl::getTransimissionBoundingBox(const Coord &senderPos, const Coord &receiverPos)
//{
//    pair<Coord,Coord> bbox;

//    bbox.first = Coord(std::min(senderPos.x, receiverPos.x), std::min(senderPos.y, receiverPos.y));
//    bbox.second = Coord(std::max(senderPos.x, receiverPos.x), std::max(senderPos.y, receiverPos.y));

//    return bbox;
//}

//double RavObstacleControl::shortestDistanceToLineSegment(const Coord &point, const Coord &lineBegin, const Coord &lineEnd)
//{
//    double px = lineEnd.x - lineBegin.x;
//    double py = lineEnd.y - lineBegin.y;

//    double ret = px * px + py * py;

//    double u = ((point.x - lineBegin.x) * px + (point.y - lineBegin.y) * py) / ret;

//    if (u > 1)
//        u = 1;
//    else if (u < 0)
//        u = 0;

//    double x = lineBegin.x + u * px;
//    double y = lineBegin.y + u * py;

//    double dx = x - point.x;
//    double dy = y - point.y;

//    double dist = sqrt(dx * dx + dy * dy);

//    return dist;
//}

double RavObstacleControl::getStreetAngle(const string & senderId, const string & receiverId)
{   
    double senderAngle = scenarioManager->getCommandInterface()->getVehicleAngle(senderId);
    double receiverAngle = scenarioManager->getCommandInterface()->getVehicleAngle(receiverId);

    return fabs(senderAngle - receiverAngle);
}

//double RavObstacleControl::getSlope(const Coord &pointA, const Coord &pointB)
//{
//    return (pointA.y - pointB.y) / (pointA.x - pointB.x);
//}

//double RavObstacleControl::getSlopeOfPerpendicularLine(double originatingLineSlope)
//{
//    return -1 (1 / originatingLineSlope);
//}

double RavObstacleControl::getMinDistanceFromPointToLine(const Coord & pointOfInterest, const Coord & linePointA, const Coord & linePointB)
{

//#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
//#define norm(v)     sqrt(dot(v,v))     // norm = length of  vector
//#define d(u,v)      norm(u-v)          // distance = norm of difference

//    // http://goo.gl/WsnSI

//    double l2 = linePointA.squareLength();

//    if (l2 == 0.0)
//        return pointOfInterest.distance(linePointA);
//    double t = ((pointOfInterest.x - linePointA.x)
//                  * (linePointB.x - linePointA.x)
//                  + (pointOfInterest.y - linePointA.y)
//                  * (linePointB.y - linePointA.y))
//                  / l2;

//    if (t<0.0)
//        return pointOfInterest.distance(linePointA);
//    else if (t>1.0)
//        return pointOfInterest.distance(linePointB);

//    Coord projection = linePointA + t * (linePointB - linePointA);

//    http://goo.gl/oR4MO8

    double A = pointOfInterest.x - linePointA.x;
    double B = pointOfInterest.y - linePointA.y;
    double C = linePointB.x - linePointA.x;
    double D = linePointB.y - linePointA.y;

    int dist = abs(A * D - C * B) / sqrt(C * C + D * D);

    return dist;
}

void RavObstacleControl::setSenderVehicleStreetSegment()
{
    AttenuationState* as = attenuationState;

    Coord sp = as->senderPosition;

    for (uint i = 0; i < as->senderStreetSegments->size(); ++i) {
        StreetSegment* ss = (*(as->senderStreetSegments))[i];

        // POINT_IS_IN_BOUNDING_BOX IS WRONG HERE !!!!!
        if (pointIsInBoundingBox(sp, ss->startPosition, ss->endPosition))
                as->senderStreetSegment = ss;
    }
}

void RavObstacleControl::setReceiverVehicleStreetSegment()
{
    AttenuationState* as = attenuationState;

    Coord sp = as->receiverPosition;

    for (uint i = 0; i < as->receiverStreetSegments->size(); ++i) {
        StreetSegment* ss = (*(as->receiverStreetSegments))[i];
        if (pointIsInBoundingBox(sp, ss->startPosition, ss->endPosition))
                as->receiverStreetSegment = ss;
    }
}

std::string RavObstacleControl::getVehicleIdFromCoord(const Coord &coord)
{
    string vehicleId;

    list<string> vehicleIds = scenarioManager->getCommandInterface()->getVehicleIds();

    for (list<string>::iterator i = vehicleIds.begin(); i != vehicleIds.end(); ++i) {
        vehicleId = *i;

        Coord vehicleCoord = scenarioManager->traci2omnet(scenarioManager->getCommandInterface()->getVehiclePosition(vehicleId));

        if ((((vehicleCoord.x - 2) < coord.x) && (coord.x < (vehicleCoord.x + 2)))
            && (((vehicleCoord.y - 2) < coord.y) && (coord.y < (vehicleCoord.y + 2)))) {
            std::cerr << "[DEBUG] vehicle " << vehicleId << " was found through coord: " << coord;
            return vehicleId;
        }
    }
    std::cerr << "[DEBUG] a vehicle was NOT FOUND with coord: " << coord;
    error("[ERROR] vehicleId not found through coord");
    return string();
}

//double RavObstacleControl::getYcoordForNewPoint(const Coord &pointA, const Coord & pointB, double newPoint_x)
//{
//    // A_y - B_y = slope(pointA, pointB) * (A_x - B_x)
//    // A_y = ((slope * A_x) - (slope * pointB.x)) + point.y
//    // newPoint_y = ((slope * newPoint_x) - slope * pointB.x)) + pointB.y
//    double slope = getSlope(pointA, pointB);

//    return ((slope * newPoint_x) - (slope * pointB.x)) + pointB.y;

//}

//double RavObstacleControl::getXcoordForNewPoint(const Coord &pointA, const Coord & pointB, double newPoint_y)
//{
//    // A_y - B_y = slope(pointA, pointB) * (A_x - B_x)
//    // double slopeRatio = slope(pointA, pointB);
//    // A_y - pointB.y = slopeRatio * (A_x - pointB.x)
//    // A_y - pointB.y = (slopeRatio * A_x) - (slopeRatio * pointB.x)
//    // A_y = ((slopeRatio * A_x) - (slopeRatio * pointB.x)) + pointB.y
//    // newPoint_y = ((slopeRatio * newPoint_x) - (slopeRatio * pointB.x)) + pointB.y
//    // ((newPoint_y / slopeRatio) - pointB.y) + (slopeRatio * point.x) = newPoint_x
//    double slope = getSlope(pointA, pointB);

//    return ((newPoint_y / slope) - pointB.y) + (slope * pointB.x);
//}





















