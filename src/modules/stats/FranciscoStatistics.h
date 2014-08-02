#ifndef FRANCISCOSTATISTICS_H
#define FRANCISCOSTATISTICS_H
#include <csimplemodule.h>

class FranciscoStatistics : public cSimpleModule
{

public:
    ~FranciscoStatistics();
    void updateAllBeaconsReceived();
    void updateAllWarningsReceived();
    void updateAllMessagesReceived();

protected:
    uint allBeaconsReceived;
    uint allWarningsReceived;
    uint allMessagesReceived;

    simsignal_t allBeaconsReceivedSignal;
    simsignal_t allWarningsReceivedSignal;
    simsignal_t allMessagesReceivedSignal;


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
