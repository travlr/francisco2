#ifndef STREET_H
#define STREET_H

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include <utility>
using std::pair;
#include <cstdlib>

#include <cxmlelement.h>
#include <Coord.h>
#include "Junction.h"
#include "JunctionManager.h"

struct Segment
{
    Coord startPosition;
    Coord endPosition;
};

typedef Segment StreetSegment;
typedef Segment LaneSegment;
typedef vector<StreetSegment*> StreetSegments;
typedef vector<LaneSegment*> LaneSegments;

struct Lane
{
    string traciId;
    string xmlId;

    string traciShape;
    string xmlShape;

    string xmlIndex;

    string xmlLength;

    LaneSegments laneSegments;

    double width;


    Lane(const string & traciLaneId) : traciId(traciLaneId) {}
    ~Lane()
    {
        for (uint i = 0; i < laneSegments.size(); ++i)
            delete laneSegments[i];
    }
};



typedef vector<Lane*> Lanes;
typedef map<string,Lane*> LaneMap;



class Street
{
public:
    Street(const string & traciId);
    ~Street();

    Lanes*      getLanes()   { return &lanes; }
    LaneMap*    getLaneMap() { return &laneMap; }

    string      getId() { return traciId; }

    Junction*   getFromJunction() { return fromJunction; }
    Junction*   getToJunction() { return toJunction; }

    StreetSegments* getStreetSegments() { return &streetSegments; }

    double getLaneWidth(int index) { return lanes[index]->width; }

    void setLaneTraciShape(int index, const string & traciLaneShape) { lanes[index]->traciShape = traciLaneShape; }

    void setXmlId(const string & xmlId) { this->xmlId = xmlId; }

    void setXmlShape(const string & xmlShape) { this->xmlShape = xmlShape; }

    void setXmlType(const string & xmlType) { this->xmlType = xmlType; }

    void setFromJunctionId(const string & fromJunctionId) { this->fromJunctionId = fromJunctionId; }

    void setToJunctionId(const string & toJunctionId) { this->toJunctionId = toJunctionId; }


protected:
    string traciId;
    string xmlId;

    string xmlShape;
    string traciShape;

    string xmlType;

    Lanes  lanes;
    LaneMap laneMap;

    string fromJunctionId;
    string toJunctionId;

    Junction* fromJunction;
    Junction* toJunction;

    StreetSegments streetSegments;






};




#endif // STREET_H
