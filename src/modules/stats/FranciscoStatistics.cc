#include "FranciscoStatistics.h"

Define_Module(FranciscoStatistics)


FranciscoStatistics::~FranciscoStatistics()
{}


void FranciscoStatistics::initialize(int stage)
{
    if (stage == 0) {

//    }
//    else if (stage == 1) {
        allBeaconsReceivedSignal = registerSignal("allBeaconsReceivedSignal");
        allWarningsReceivedSignal = registerSignal("allWarningsReceivedSignal");
        allMessagesReceivedSignal = registerSignal("allMessagesReceivedSignal");
    }
}


void FranciscoStatistics::finish()
{
}


void FranciscoStatistics::updateAllBeaconsReceived()
{
    emit(allBeaconsReceivedSignal, ++allBeaconsReceived);
}

void FranciscoStatistics::updateAllWarningsReceived()
{
    emit(allWarningsReceivedSignal, ++allWarningsReceived);
}

void FranciscoStatistics::updateAllMessagesReceived()
{
    emit(allMessagesReceivedSignal, ++allMessagesReceived);
}


