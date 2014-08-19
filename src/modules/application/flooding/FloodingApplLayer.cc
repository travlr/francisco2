#include "FloodingApplLayer.h"
#include <iostream>

using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;
using std::cerr;

Define_Module(FloodingApplLayer)

void FloodingApplLayer::initialize(int stage)
{
	BaseWaveApplLayer::initialize(stage);
	if (stage == 0) {
		traci = TraCIMobilityAccess().get(getParentModule());
		annotations = AnnotationManagerAccess().getIfExists();
		ASSERT(annotations);
        stats = FranciscoStatisticsAccess().getIfExists();
        ASSERT(stats);

        beaconReceivedSignal = registerSignal("beaconReceivedSignal");
        warningReceivedSignal = registerSignal("warningReceivedSignal");
        messageReceivedSignal = registerSignal("messageReceivedSignal");
        newWarningReceivedSignal = registerSignal("newWarningReceivedSignal");


		sentMessage = false;
		lastDroveAt = simTime();
	}
    else if (stage == 1) {

    }
}

void FloodingApplLayer::finish()
{
    for (uint i = 0; i < warningMessages.size(); ++i) {
        delete warningMessages[i];
    }
    warningMessages.clear();;
}

void FloodingApplLayer::onBeacon(WaveShortMessage* wsm)
{
    stats->updateAllBeaconsReceived();
    stats->updateAllMessagesReceived();
    emit(beaconReceivedSignal, 1);
    emit(messageReceivedSignal, 1);
}

void FloodingApplLayer::onData(WaveShortMessage* wsm)
{
    emit(warningReceivedSignal, 1);
    emit(messageReceivedSignal, 1);
    stats->updateAllWarningsReceived();
    stats->updateAllMessagesReceived();

    bool messageIsRepeat = false;

    for (uint i = 0; i < warningMessages.size(); ++i) {
        if (wsm->getTreeId() == warningMessages[i]->getTreeId()) {
            messageIsRepeat = true;
        }
    }

    if (!messageIsRepeat) {

        findHost()->getDisplayString().updateWith("r=16,green");
        annotations->scheduleErase(1, annotations->drawLine(wsm->getSenderPos(), traci->getPositionAt(simTime()), "blue"));

        if (traci->getRoadId()[0] != ':')
            traci->commandChangeRoute(wsm->getWsmData(), 9999);

        sendMessage(wsm->getWsmData());
        stats->updateNewWarningsReceived();
        emit(newWarningReceivedSignal, 1);

        warningMessages.push_back(wsm->dup());

    }
}

void FloodingApplLayer::sendMessage(std::string blockedRoadId)
{
	sentMessage = true;

	t_channel channel = dataOnSch ? type_SCH : type_CCH;
	WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
	wsm->setWsmData(blockedRoadId.c_str());
	sendWSM(wsm);
}


void FloodingApplLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj)
{
	Enter_Method_Silent();
	if (signalID == mobilityStateChangedSignal) {
		handlePositionUpdate(obj);
	}
}


void FloodingApplLayer::handlePositionUpdate(cObject* obj)
{
	BaseWaveApplLayer::handlePositionUpdate(obj);

	// stopped for for at least 10s?
	if (traci->getSpeed() < 1) {
		if (simTime() - lastDroveAt >= 10) {
			findHost()->getDisplayString().updateWith("r=16,red");
            stats->updateNewWarningsReceived();
			if (!sentMessage) sendMessage(traci->getRoadId());
		}
	}
	else {
		lastDroveAt = simTime();
	}
}


void FloodingApplLayer::sendWSM(WaveShortMessage* wsm)
{
	sendDelayedDown(wsm,individualOffset);
}
