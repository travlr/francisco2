package src.modules.stats;


simple FranciscoStatistics
{
    parameters:
        @class(FranciscoStatistics);
        @display("i=block/table2_vl.png");

        @signal[allBeaconsReceivedSignal](type=long);
        @statistic[allBeaconsReceivedSignal](record=count,vector; description="All Beacons Received");

        @signal[allWarningsReceivedSignal](type=long);
        @statistic[allWarningsReceivedSignal](record=count,vector; description="All Warnings Received");

        @signal[allMessagesReceivedSignal](type=long);
        @statistic[allMessagesReceivedSignal](record=count,vector; description="All Messages Received");
}
