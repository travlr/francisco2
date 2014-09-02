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
        virtual void handlePositionUpdate(cObject* obj);
        virtual void sendWSM(WaveShortMessage* wsm);
        virtual void onBeacon(WaveShortMessage* wsm);
        virtual void onData(WaveShortMessage* wsm);

	protected:
        int accidentCount;

		TraCIMobility* traci;
		AnnotationManager* annotations;
		simtime_t lastDroveAt;
        bool sentMessage;

        simsignal_t beaconReceivedSignal;
        simsignal_t newWarningReceivedSignal;
        simsignal_t warningReceivedSignal;
        simsignal_t messageReceivedSignal;

        FranciscoStatistics* stats;
        vector<WaveShortMessage*> warningMessages;

	protected:
        void sendNewWarningMessage(std::string blockedRoadId);

};

#endif
