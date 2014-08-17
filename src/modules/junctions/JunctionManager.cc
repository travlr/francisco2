#include "JunctionManager.h"
#include "Junction.h"
using std::string;
using std::vector;
#include <cstdio>
using std::sscanf;



void JunctionManager::initialize(int stage)
{
    if (stage == 0) {

    }
}


void JunctionManager::addJunction(cXMLElement *junctionEl)
{
    Junction* j = new Junction(junctionEl);
    junctions.push_back(j);
    junctionMap[j->getId()] = j;
}

