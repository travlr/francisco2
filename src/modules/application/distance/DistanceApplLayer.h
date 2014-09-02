#ifndef DistanceApplLayer_H
#define DistanceApplLayer_H

#include <BaseWaveApplLayer.h>
#include <TraCIMobility.h>
#include "FranciscoStatistics.h"
#include <map>
#include <vector>

typedef std::vector<WaveShortMessage*> WaveShortMessages;

class DistanceApplLayer : public BaseWaveApplLayer
{

public:
    virtual void initialize(int stage);
    virtual void finish();
    virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj);
    virtual void handlePositionUpdate(cObject* obj);
    virtual void sendWSM(WaveShortMessage* wsm);
    virtual void onBeacon(WaveShortMessage* wsm);
    virtual void onData(WaveShortMessage* wsm);

protected:
    void sendNewWarningMessage(std::string blockedRoadId);


protected:
    Veins::TraCIMobility* traci;
    FranciscoStatistics* stats;
    simtime_t lastDroveAt;
    bool sentMessage;
    double distanceThreshold;
    int accidentCount;

    simsignal_t warningReceived;

    simsignal_t beaconReceivedSignal;
    simsignal_t newWarningReceivedSignal;
    simsignal_t warningReceivedSignal;
    simsignal_t messageReceivedSignal;

    std::map<long,WaveShortMessages> receivedMessages;   // treeId, WSMvector
};

#endif // DistanceApplLayer_H
