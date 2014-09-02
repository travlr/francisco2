#include "DistanceApplLayer.h"

using Veins::TraCIMobility;
using Veins::TraCIMobilityAccess;
using std::map;
using std::vector;


Define_Module(DistanceApplLayer)



void DistanceApplLayer::initialize(int stage) {

    BaseWaveApplLayer::initialize(stage);

    if (stage == 0) {

        distanceThreshold = par("distanceThreshold").doubleValue();
        accidentCount = par("accidentCount").longValue();

        traci = TraCIMobilityAccess().get(getParentModule());
//		annotations = AnnotationManagerAccess().getIfExists();
//		ASSERT(annotations);
        stats = FranciscoStatisticsAccess().getIfExists();
        ASSERT(stats);

        beaconReceivedSignal = registerSignal("beaconReceivedSignal");
        warningReceivedSignal = registerSignal("warningReceivedSignal");
        messageReceivedSignal = registerSignal("messageReceivedSignal");
        newWarningReceivedSignal = registerSignal("newWarningReceivedSignal");

        sentMessage = false;
        lastDroveAt = simTime();
    }
}

void DistanceApplLayer::finish()
{

}

void DistanceApplLayer::onBeacon(WaveShortMessage *wsm)
{}


void DistanceApplLayer::onData(WaveShortMessage *wsm)
{
    emit(warningReceivedSignal, 1);
    emit(messageReceivedSignal, 1);
    stats->updateAllWarningsReceived();
    stats->updateAllMessagesReceived();

    Coord coord = traci->getPositionAt(simTime());

    // dMin == the shortest d of a any neighbor who has broadcast the same message

    // add message to receivedMessages
    receivedMessages[wsm->getTreeId()].push_back(wsm->dup());

    // get dMin
    WaveShortMessages mv = receivedMessages[wsm->getTreeId()];
    double dMin = 100000.0;
    WaveShortMessage* dMinMessage;

    // is it a new warning message?
    if (mv.size() == 1) {
        stats->updateNewWarningsReceived();
        emit(newWarningReceivedSignal, 1);
    }

    for (uint i = 0; i < mv.size(); ++i) {
        WaveShortMessage* m = mv[i];
        double d = coord.distance(m->getSenderPos());
        if (d < dMin) {
            dMin = d;
            dMinMessage = m;
        }
    }

    // rebroadcast?
    bool rebroadcast = false;
    if (dMin > distanceThreshold)
        rebroadcast = true;

    if (rebroadcast) {
        sendWSM(dMinMessage->dup());

    }
}


void DistanceApplLayer::sendNewWarningMessage(std::string blockedRoadId)
{
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
    wsm->setWsmData(blockedRoadId.c_str());
    sendWSM(wsm);
}

void DistanceApplLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj) {
    Enter_Method_Silent();
    if (signalID == mobilityStateChangedSignal) {
        handlePositionUpdate(obj);
    }
}

void DistanceApplLayer::handlePositionUpdate(cObject* obj)
{
    BaseWaveApplLayer::handlePositionUpdate(obj);

    // stopped for for at least 10s?
    if ((!sentMessage) && (traci->getSpeed() < 1)) {

        if (simTime() - lastDroveAt >= 10) {

            // HACK to fix stack's broken implementation

            if (stats->getNumberOfAccidentsOccurred() == accidentCount) {
                lastDroveAt = simTime();
                return;
            }
            else {
                stats->incrementAccidentOccurred();
            }

            // END HACK

            findHost()->getDisplayString().updateWith("r=16,red");

            stats->updateNewWarningsReceived();

            sendNewWarningMessage(traci->getRoadId());

            sentMessage = true;
        }
    }
    else {
        lastDroveAt = simTime();
    }
}
void DistanceApplLayer::sendWSM(WaveShortMessage* wsm) {
    sendDelayedDown(wsm,individualOffset);
}




