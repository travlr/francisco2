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
#include "Street.h"

#define GRIDCELL_SIZE 1024


Define_Module(RavObstacleControl)

using Veins::AnnotationManagerAccess;

void RavObstacleControl::initialize(int stage) {
    if (stage == 1)	{

		annotations = AnnotationManagerAccess().getIfExists();
        if (annotations)
            annotationGroup = annotations->createGroup("obstacles");

        streetAngularDifferenceThreshold                = par("streetAngularDifferenceThreshold");
        distanceFromVehicleToStreetProjectionThreshold  = par("distanceFromVehicleToStreetProjectionThreshold");
        distanceOfVehicleFromJunctionThreshold          = par("distanceOfVehicleFromJunctionThreshold");

        scenarioManager = TraCIScenarioManagerLaunchdAccess().get();

        attenuationState = new AttenuationState;

        cModule* mod = simulation.getSystemModule()->getSubmodule("roadmapmgr");
        if (!mod)
            error("[ERROR] could not get module roadmapmgr");
        roadMapManager = check_and_cast<RoadMapManager*>(mod);
	}
}


void RavObstacleControl::finish() {
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
    if (!roadMapManager->getJunctionsAreLoaded())
        roadMapManager->loadJunctions();

    if (!roadMapManager->getStreetsAreLoaded())
        roadMapManager->loadStreets();

    memset(attenuationState, 0, sizeof(*attenuationState));
    AttenuationState* as = attenuationState;

    as->frame = frame;
    as->senderPosition = senderPos;
    as->receiverPosition = receiverPos;

    // a hack to get the vehicle's id string
    strcpy(as->senderIdStr, getVehicleIdFromCoord(senderPos).c_str());
    strcpy(as->receiverIdStr, getVehicleIdFromCoord(receiverPos).c_str());

    as->streetAngle = getStreetAngle(as->senderIdStr, as->receiverIdStr);

    strcpy(as->senderRoadId, scenarioManager->getCommandInterface()->getVehicleRoadId(as->senderIdStr).c_str());
    strcpy(as->receiverRoadId, scenarioManager->getCommandInterface()->getVehicleRoadId(as->receiverIdStr).c_str());

    as->senderStreet = roadMapManager->getStreetMap()[as->senderIdStr];
    as->receiverStreet = roadMapManager->getStreetMap()[as->receiverIdStr];

    if (!as->senderStreet)
        error("[ERROR] could not get senderStreet, streetMap.size(): %d", roadMapManager->getStreetMap().size());
    if (!as->receiverStreet)
        error("[ERROR] could not get receiverStreet, streetMap.size(): %d", roadMapManager->getStreetMap().size());


    setSenderVehicleStreetSegment();
    setReceiverVehicleStreetSegment();
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
    Junction* from = as->senderStreet->getFromJunction();
    Junction* to = as->senderStreet->getToJunction();

    if (as->senderPosition.distance(from->getPosition()) < distanceOfVehicleFromJunctionThreshold) {
        as->junctionSenderIsIn = from;
        return true;
    }

    if (as->senderPosition.distance(to->getPosition()) < distanceOfVehicleFromJunctionThreshold) {
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

    StreetSegments &sss = as->senderStreet->getStreetSegments();

    cerr << "[DEEEEEBBBBBBBBUUUUUUUUUUGGGGGGG] " << sss.size() << std::endl;

    //for (uint i = 0; i < as->senderStreet->getStreetSegments()->size(); ++i) {
    for (uint i = 0; i < sss.size(); ++i) {
        StreetSegment* ss = sss[i];
        if (pointIsInBoundingBox(sp, ss->startPosition, ss->endPosition))
                as->senderStreetSegment = ss;
    }
}

void RavObstacleControl::setReceiverVehicleStreetSegment()
{
    AttenuationState* as = attenuationState;

    Coord sp = as->receiverPosition;

    StreetSegments rss = as->receiverStreet->getStreetSegments();

    for (uint i = 0; i < rss.size(); ++i) {
        StreetSegment* ss = rss[i];
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





















