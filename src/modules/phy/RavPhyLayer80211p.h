#ifndef RAVPHYLAYER80211P_H
#define RAVPHYLAYER80211P_H

#include <PhyLayer80211p.h>
#include "RoadMapManager.h"

class RavPhyLayer80211p : public PhyLayer80211p
{

public:
    void initialize(int stage);

protected:
    AnalogueModel* initializeRavObstacleModel(ParameterMap & params);
    AnalogueModel* getAnalogueModelFromName(std::string name, ParameterMap &params);

protected:
    RoadMapManager* roadMapManager;
};

#endif // RAVPHYLAYER80211P_H
