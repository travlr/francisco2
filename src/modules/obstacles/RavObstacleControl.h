#ifndef RAV_OBSTACLECONTROL_H
#define RAV_OBSTACLECONTROL_H

#include <list>
using std::list;
#include <utility>
using std::pair;
#include <string>
using std::string;

#include <omnetpp.h>
#include <Coord.h>
#include "Street.h"
#include <AnnotationManager.h>
#include <TraCIScenarioManagerLaunchd.h>
namespace Veins {
class AirFrame;
}

using Veins::AirFrame;

using Veins::AnnotationManager;

class RoadMapManager;
class Junction;


class RavObstacleControl : public cSimpleModule
{
	public:
        ~RavObstacleControl() {}
		void initialize(int stage);
		int numInitStages() const { return 2; }
		void finish();
		void handleMessage(cMessage *msg);
        void handleSelfMsg(cMessage *msg);

		/**
		 * calculate additional attenuation by obstacles, return signal strength
		 */
        double calculateAttenuation(const Veins::AirFrame *frame, const Coord& senderPos, const Coord& receiverPos);

    protected:
        struct AttenuationState
        {
            const AirFrame* frame;

            Coord senderPosition;
            Coord receiverPosition;

            bool areOnSameStreet;
            bool areOnSameStreetSegment;
            bool senderIsInJunction;
            bool receiverIsInJunction;
            bool areOnAdjacentStreets;

            double streetAngle;

            char senderIdStr[64];
            char receiverIdStr[64];

            char senderRoadId[64];
            char receiverRoadId[64];

            double senderStreetWidth;
            double receiverStreetWidth;

            Street* senderStreet;
            Street* receiverStreet;

            Junction* junctionSenderIsIn;                       // if NULL, not in junction
            Junction* sharedJunction;

            StreetSegment* senderStreetSegment;
            StreetSegment* receiverStreetSegment;
        };

		AnnotationManager* annotations;
		AnnotationManager::Group* annotationGroup;

        double streetAngularDifferenceThreshold;
        double distanceFromVehicleToStreetProjectionThreshold;
        double distanceOfVehicleFromJunctionThreshold;

        Veins::TraCIScenarioManagerLaunchd*     scenarioManager;
        RoadMapManager*                         roadMapManager;

        AttenuationState* attenuationState;


protected:
        void initializeAttenuationState(const Veins::AirFrame* frame, const Coord & senderPos, const Coord & receiverPos);
        bool vehiclesHaveLineOfSight();
        bool senderVehicleIsInJunction();
        bool vehicleStreetsAreAdjacent();

        bool pointIsInBoundingBox(const Coord & point, const Coord & startPoint, const Coord & endPoint);
//        pair<Coord,Coord> getTransimissionBoundingBox(const Coord & senderPos, const Coord & receiverPos);
//        double shortestDistanceToLineSegment(const Coord & point, const Coord & lineBegin, const Coord & lineEnd);
        double getStreetAngle(const std::string &senderId, const std::string &receiverId);
//        double getSlope(const Coord & pointA, const Coord & pointB);
//        double getSlopeOfPerpendicularLine(double originatingLineSlope);
//        double findPointBxGivenCoordAandBy(const Coord & pointA, double pointB_y);
//        double getPointByGivenCoordAandBx(const Coord & pointA, double pointB_x);
        double getMinDistanceFromPointToLine(const Coord & pointOfInterest, const Coord & linePointA, const Coord & linePointB);
        void setSenderVehicleStreetSegment();
        void setReceiverVehicleStreetSegment();
//        double getYcoordForNewPoint(const Coord &pointA, const Coord & pointB, double newPoint_x);
        string getVehicleIdFromCoord(const Coord & coord);

};


class RavObstacleControlAccess
{
	public:
        RavObstacleControlAccess() {
		}

        RavObstacleControl* getIfExists() {
            return dynamic_cast<RavObstacleControl*>(simulation.getModuleByPath("obstacles"));
		}
};


#endif

