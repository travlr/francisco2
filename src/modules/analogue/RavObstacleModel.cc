#include "RavObstacleModel.h"

using Veins::AirFrame;

#define debugEV (ev.isDisabled() || !debug) ? ev : ev << "PhyLayer(RavObstacleModel): "

#if 0
SimplePathlossConstMapping::SimplePathlossConstMapping(const DimensionSet& dimensions,
                                                       SimpleObstacleShadowing* model,
                                                       const double distFactor) :
    SimpleConstMapping(dimensions),
    distFactor(distFactor),
    model(model),
    hasFrequency(dimensions.hasDimension(Dimension::frequency_static()))
{
}

double SimplePathlossConstMapping::getValue(const Argument& pos) const
{
    double freq = model->carrierFrequency;
    if(hasFrequency) {
        assert(pos.hasArgVal(Dimension::frequency));
        freq = pos.getArgValue(Dimension::frequency);
    }
    double wavelength = BaseWorldUtility::speedOfLight / freq;
    return (wavelength * wavelength) * distFactor;
}
#endif

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

    double factor = obstacleControl.calculateAttenuation(senderPos, receiverPos);

    debugEV << "value is: " << factor << endl;

    bool hasFrequency = s.getTransmissionPower()->getDimensionSet().hasDimension(Dimension::frequency);

    const DimensionSet & domain = hasFrequency ? DimensionSet::timeFreqDomain : DimensionSet::timeDomain;
    ConstantSimpleConstMapping* attMapping = new ConstantSimpleConstMapping(domain, factor);
    s.addAttenuation(attMapping);
}
