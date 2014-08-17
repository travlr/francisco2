#ifndef RAVOBSTACLEMODEL_H
#define RAVOBSTACLEMODEL_H

#include <AnalogueModel.h>
#include <Mapping.h>
#include <BaseWorldUtility.h>
#include "RavObstacleControl.h"
#include <Move.h>
#include <Signal_.h>
#include <AirFrame_m.h>

using Veins::AirFrame;

#include  <cstdlib>


class RavObstacleModel : public AnalogueModel
{

public:
    RavObstacleModel(double carrierFrequency, bool useTorus, const Coord & playgroundSize, bool debug);
    virtual void filterSignal (AirFrame *frame, const Coord &senderPos, const Coord &receiverPos);

protected:
//    RavObstacleControl & obstacleControl;
    double carrierFrequency;
    const bool useTorus;
    const Coord & playgroundSize;
    bool debug;

protected:
};

#endif // RAVOBSTACLEMODEL_H
