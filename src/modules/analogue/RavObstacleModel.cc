#include "RavObstacleModel.h"


using Veins::AirFrame;


RavObstacleModel::RavObstacleModel(RavObstacleControl & obstacleControl, double carrierFrequency, bool useTorus, const Coord & playgroundSize, bool debug)
    : obstacleControl(obstacleControl)
    , carrierFrequency(carrierFrequency)
    , useTorus(useTorus)
    , playgroundSize(playgroundSize)
    , debug(debug)
{
    if (useTorus)
        opp_error("[ERROR] RavObstacleModel does not work on torus-shaped playgrounds");
}


void RavObstacleModel::filterSignal(Veins::AirFrame *frame, const Coord &senderPos, const Coord &receiverPos)
{
    Signal & s = frame->getSignal();

    double factor = obstacleControl.calculateAttenuation(frame, senderPos, receiverPos);

    bool hasFrequency = s.getTransmissionPower()->getDimensionSet().hasDimension(Dimension::frequency);

    const DimensionSet & domain = hasFrequency ? DimensionSet::timeFreqDomain : DimensionSet::timeDomain;
    ConstantSimpleConstMapping* attMapping = new ConstantSimpleConstMapping(domain, factor);
    s.addAttenuation(attMapping);
}
