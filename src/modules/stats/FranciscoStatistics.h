#ifndef FRANCISCOSTATISTICS_H
#define FRANCISCOSTATISTICS_H
#include <csimplemodule.h>

class FranciscoStatistics : public cSimpleModule
{

public:
    void updateAllBeaconsReceived();
    void updateNewWarningsReceived();
    void updateAllWarningsReceived();
    void updateAllMessagesReceived();

    int getNumberOfAccidentsOccurred() { return numAccidentsOccurred; }
    void incrementAccidentOccurred();

protected:
    uint allBeaconsReceived;
    uint newWarningsReceived;
    uint allWarningsReceived;
    uint allMessagesReceived;
//    bool accidentMode;
//    int numVehicles;

    int numAccidentsOccurred;

    simsignal_t allBeaconsReceivedSignal;
    simsignal_t allNewWarningsReceivedSignal;
    simsignal_t allWarningsReceivedSignal;
    simsignal_t allMessagesReceivedSignal;
    simsignal_t numAccidentsSignal;

protected:
    virtual void initialize(int stage);
//    virtual int numInitStages() const { return 2; }
    virtual void finish();


};

class FranciscoStatisticsAccess
{
    public:
        FranciscoStatisticsAccess() {
        }

        FranciscoStatistics* getIfExists() {
            return dynamic_cast<FranciscoStatistics*>(simulation.getModuleByPath("stats"));
        }
};

#endif // FRANCISCOSTATISTICS_H
