#ifndef JUNCTIONMANAGER_H
#define JUNCTIONMANAGER_H

#include <vector>
#include <map>
#include <string>
#include <csimplemodule.h>

using std::vector;
using std::map;
using std::string;

class Junction;

typedef vector<Junction*> Junctions;
typedef map<string, Junction*> JunctionMap;

class JunctionManager : public cSimpleModule
{

public:
    void addJunction(cXMLElement* junctionEl);
    Junctions* getJunctions()  { return &junctions; }
    JunctionMap* getJunctionMap() { return &junctionMap; }

protected:
    Junctions junctions;
    JunctionMap junctionMap;

protected:
    virtual void initialize(int stage);
};

#endif // JUNCTIONMANAGER_H
