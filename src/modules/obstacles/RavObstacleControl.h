//
// RavObstacleControl - models obstacles that block radio transmissions
// Copyright (C) 2006 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
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
#include "RavObstacle.h"
#include "Street.h"
#include <AnnotationManager.h>
#include <TraCIScenarioManagerLaunchd.h>
namespace Veins {
class AirFrame;
}

class RoadMapManager;
class Junction;

/**
 * RavObstacleControl models obstacles that block radio transmissions.
 *
 * Each RavObstacle is a polygon.
 * Transmissions that cross one of the polygon's lines will have
 * their receive power set to zero.
 */

class RavObstacleControl : public cSimpleModule
{
	public:
        ~RavObstacleControl() {}
		void initialize(int stage);
		int numInitStages() const { return 2; }
		void finish();
		void handleMessage(cMessage *msg);
        void handleSelfMsg(cMessage *msg);

		void addFromXml(cXMLElement* xml);
        void add(RavObstacle obstacle);
        void erase(const RavObstacle* obstacle);

		/**
		 * calculate additional attenuation by obstacles, return signal strength
		 */
        double calculateAttenuation(const Veins::AirFrame *frame, const Coord& senderPos, const Coord& receiverPos);

    protected:
        struct AttenuationState
        {
                bool areOnSameStreet;
                bool areOnSameStreetSegment;
                bool senderIsInJunction;
                bool receiverIsInJunction;
                bool areOnAdjacentStreets;

                double streetAngle;

                int senderId;
                int receiverId;

                char senderIdStr[64];
                char receiverIdStr[64];

                char senderRoadId[64];
                char receiverRoadId[64];

                double senderStreetWidth;
                double receiverStreetWidth;

                Coord senderPosition;
                Coord receiverPosition;

                Street* senderStreet;
                Street* receiverStreet;

                Junction* junctionSenderIsIn;                       // if NULL, not in junction
                Junction* sharedJunction;

                StreetSegments* senderStreetSegments;
                StreetSegments* receiverStreetSegments;

                StreetSegment* senderStreetSegment;
                StreetSegment* receiverStreetSegment;
        };

//		struct CacheKey {
//			const Coord senderPos;
//			const Coord receiverPos;

//			CacheKey(const Coord& senderPos, const Coord& receiverPos) :
//				senderPos(senderPos),
//				receiverPos(receiverPos) {
//			}

//			bool operator<(const CacheKey& o) const {
//				if (senderPos.x < o.senderPos.x) return true;
//				if (senderPos.x > o.senderPos.x) return false;
//				if (senderPos.y < o.senderPos.y) return true;
//				if (senderPos.y > o.senderPos.y) return false;
//				if (receiverPos.x < o.receiverPos.x) return true;
//				if (receiverPos.x > o.receiverPos.x) return false;
//				if (receiverPos.y < o.receiverPos.y) return true;
//				if (receiverPos.y > o.receiverPos.y) return false;
//				return false;
//			}
//		};

//		enum { GRIDCELL_SIZE = 1024 };

        typedef std::list<RavObstacle*> RavObstacleGridCell;
        typedef std::vector<RavObstacleGridCell> RavObstacleGridRow;
        typedef std::vector<RavObstacleGridRow> RavObstacles;
//		typedef std::map<CacheKey, double> CacheEntries;

//		bool debug; /**< whether to emit debug messages */
		cXMLElement* obstaclesXml; /**< obstacles to add at startup */

        RavObstacles obstacles;
		AnnotationManager* annotations;
		AnnotationManager::Group* annotationGroup;
//		mutable CacheEntries cacheEntries;

        double streetAngularDifferenceThreshold;
        double distanceFromVehicleToStreetProjectionThreshold;
        double distanceOfVehicleFromJunctionThreshold;

        Veins::TraCIScenarioManagerLaunchd* scenarioManager;
        RoadMapManager* roadMapManager;

        //AttenuationState* attenuationState;


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

