#include "Street.h"
#include <cstring>
#include <vector>
#include <cstdlib>

using std::string;
using std::vector;

Street::Street(const string & id)
    : id(id)
{

}

Street::~Street()
{
    for (uint i = 0; i < lanes.size(); ++i) {
        delete lanes[i];
    }
}

void Street::addLane(const std::string &laneId)
{
    Lane* l = new Lane(laneId);
    lanes.push_back(l);
    laneMap[laneId] = l;
}

void Street::setLaneShape(const std::string &laneId, const list<Coord> &shapeList)
{
    for (list<Coord>::const_iterator i = shapeList.begin(); i != shapeList.end(); ++i) {
        Coord coord = *i;
        laneMap[laneId]->shapeCoords.push_back(coord);
    }

    Lane* l = laneMap[laneId];
    for (uint i = 0; i < l->shapeCoords.size()-1; ++i) {
        StreetSegment* ss = new StreetSegment;
        ss->startPosition = l->shapeCoords[i];
        ss->endPosition = l->shapeCoords[i+1];
        streetSegments.push_back(ss);
    }
}



