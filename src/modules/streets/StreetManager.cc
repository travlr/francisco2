#include "StreetManager.h"
#include "Street.h"

StreetManager::StreetManager()
{
}

StreetManager::~StreetManager()
{
    for (uint i = 0; i < streets.size(); ++i) {
        delete streets[i];
    }
}

void StreetManager::addStreet(const string & traciId)
{
    Street* s = new Street(traciId);
    streets.push_back(s);
    streetMap[s->getId()] = s;
}

void StreetManager::addLane(const std::string &traciId, const std::string &traciLaneId)
{
    Street* s = streetMap[traciId];
    s->getLanes()->push_back(new Lane(traciLaneId));
}
