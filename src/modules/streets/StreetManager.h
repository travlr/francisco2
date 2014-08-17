#ifndef STREETMANAGER_H
#define STREETMANAGER_H

#include <cxmlelement.h>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;

class Street;

typedef vector<Street*> Streets;
typedef map<string, Street*> StreetMap;

class StreetManager
{

public:
    StreetManager();
    ~StreetManager();

    void addStreet(const string &traciId);
    void addLane(const string & traciId, const string & traciLaneId);
    Streets* getStreets() { return &streets; }
    StreetMap* getStreetMap() { return &streetMap; }

protected:
    Streets streets;
    StreetMap streetMap;
};

#endif // STREETMANAGER_H
