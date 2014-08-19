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
#include <list>
using std::list;
#include <cstdlib>

#include <cxmlelement.h>
#include <Coord.h>
#include "Junction.h"


struct Lane
{
    string id;

    vector<Coord> shapeCoords;

    int index;

    double width;


    Lane(const string & id) : id(id) {}
    ~Lane() {}
};

struct StreetSegment
{
    Coord startPosition;
    Coord endPosition;
};


typedef vector<Lane*> Lanes;
typedef map<string,Lane*> LaneMap;
typedef vector<StreetSegment*> StreetSegments;

class Street
{
public:
    Street(const string & id);
    ~Street();

    Lanes*      getLanes()   { return &lanes; }
    LaneMap*    getLaneMap() { return &laneMap; }

    string      getId() { return id; }

    void        addLane(const std::string & laneId);

    void        setLaneWidth(const string & laneId, double width) { laneMap[laneId]->width = width; }

    void        setLaneShape(const string & laneId, const list<Coord> & shapeList);

    void        setLaneIndex(const string & laneId, int index) { laneMap[laneId]->index = index; }


    Junction*   getFromJunction() { return fromJunction; }
    void        setFromJunction(Junction* fromJunction) { this->fromJunction = fromJunction; }
    Junction*   getToJunction() { return toJunction; }
    void        setToJunction(Junction* toJunction) { this->toJunction = toJunction; }

    StreetSegments & getStreetSegments() { return streetSegments; }

//    double getLaneWidth(int index) { return lanes[index]->width; }

//    void setLaneTraciShape(int index, const string & traciLaneShape) { lanes[index]->traciShape = traciLaneShape; }

//    void setXmlId(const string & xmlId) { this->xmlId = xmlId; }

//    void setXmlShape(const string & xmlShape) { this->xmlShape = xmlShape; }

//    void setXmlType(const string & xmlType) { this->xmlType = xmlType; }

//    void setFromJunctionId(const string & fromJunctionId) { this->fromJunctionId = fromJunctionId; }

//    void setToJunctionId(const string & toJunctionId) { this->toJunctionId = toJunctionId; }


protected:
    string id;

    Lanes lanes;
    LaneMap laneMap;

    Junction* fromJunction;
    Junction* toJunction;

    StreetSegments streetSegments;

//    string traciId;
//    string xmlId;

//    string xmlShape;
//    string traciShape;

//    string xmlType;

//    Lanes  lanes;
//    LaneMap laneMap;

//    string fromJunctionId;
//    string toJunctionId;

//    Junction* fromJunction;
//    Junction* toJunction;

//    StreetSegments streetSegments;






};




#endif // STREET_H
