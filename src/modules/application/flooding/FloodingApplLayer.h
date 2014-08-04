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

#ifndef FloodingApplLayer_H
#define FloodingApplLayer_H

#include "FranciscoStatistics.h"
#include <BaseWaveApplLayer.h>
#include <TraCIMobility.h>
#include <vector>

using std::vector;
using Veins::TraCIMobility;
using Veins::AnnotationManager;


class FloodingApplLayer : public BaseWaveApplLayer
{
	public:
		virtual void initialize(int stage);
        virtual void finish();
		virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj);
	protected:
		TraCIMobility* traci;
		AnnotationManager* annotations;
		simtime_t lastDroveAt;
		bool sentMessage;

        simsignal_t warningReceived;

        simsignal_t beaconReceivedSignal;
        simsignal_t newWarningReceivedSignal;
        simsignal_t warningReceivedSignal;
        simsignal_t messageReceivedSignal;

        FranciscoStatistics* stats;
        vector<WaveShortMessage*> warningMessages;

	protected:
		virtual void onBeacon(WaveShortMessage* wsm);
		virtual void onData(WaveShortMessage* wsm);
		void sendMessage(std::string blockedRoadId);
		virtual void handlePositionUpdate(cObject* obj);
		virtual void sendWSM(WaveShortMessage* wsm);
};

#endif
