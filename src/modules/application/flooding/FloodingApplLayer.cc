//
// Copyright (C) 2006-2011 Christoph Sommer <christoph.sommer@uibk.ac.at>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "FloodingApplLayer.h"
#include <iostream>

using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;
using std::cerr;

Define_Module(FloodingApplLayer)

void FloodingApplLayer::initialize(int stage) {
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

void FloodingApplLayer::onBeacon(WaveShortMessage* wsm) {
    stats->updateAllBeaconsReceived();
    stats->updateAllMessagesReceived();
    emit(beaconReceivedSignal, 1);
    emit(messageReceivedSignal, 1);
}

void FloodingApplLayer::onData(WaveShortMessage* wsm) {
    emit(warningReceivedSignal, 1);
    emit(messageReceivedSignal, 1);
    stats->updateAllWarningsReceived();
    stats->updateAllMessagesReceived();
    bool messageIsRepeat = false;

    for (uint i = 0; i < warningMessages.size(); ++i) {
        //cerr << "wsmTreeId: " << wsm->getTreeId() << " warningMessages[" << i << "].treeId: " << warningMessages[i]->getTreeId() << endl;
        if (wsm->getTreeId() == warningMessages[i]->getTreeId()) {
//            EV << "[INFO] _________REPEAT MESSAGE_________";
            messageIsRepeat = true;
        }
    }

//    cerr << warningMessages.size() << " ";

    if (!messageIsRepeat) {

//        cerr << "`";

        findHost()->getDisplayString().updateWith("r=16,green");
        annotations->scheduleErase(1, annotations->drawLine(wsm->getSenderPos(), traci->getPositionAt(simTime()), "blue"));

        if (traci->getRoadId()[0] != ':')
            traci->commandChangeRoute(wsm->getWsmData(), 9999);

        sendMessage(wsm->getWsmData());
        stats->updateNewWarningsReceived();
        emit(newWarningReceivedSignal, 1);

        warningMessages.push_back(wsm->dup());

    }
//    else {
//        cerr << ".";
//    }
}

void FloodingApplLayer::sendMessage(std::string blockedRoadId) {
	sentMessage = true;

	t_channel channel = dataOnSch ? type_SCH : type_CCH;
	WaveShortMessage* wsm = prepareWSM("data", dataLengthBits, channel, dataPriority, -1,2);
	wsm->setWsmData(blockedRoadId.c_str());
	sendWSM(wsm);
}
void FloodingApplLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj) {
	Enter_Method_Silent();
	if (signalID == mobilityStateChangedSignal) {
		handlePositionUpdate(obj);
	}
//	else if (signalID == parkingStateChangedSignal) {
//		handleParkingUpdate(obj);
//	}
}

void FloodingApplLayer::handlePositionUpdate(cObject* obj) {
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
void FloodingApplLayer::sendWSM(WaveShortMessage* wsm) {
	sendDelayedDown(wsm,individualOffset);
}
