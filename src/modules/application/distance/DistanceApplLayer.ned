package src.modules.application.distance;

import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;


simple DistanceApplLayer extends BaseWaveApplLayer
{
    @class(DistanceApplLayer);
    @display("i=block/app2");

    double distanceThreshold = default(30.0);

    @signal[warningReceivedSignal](type=long);
    @statistic[warningReceivedSignal](record=count,vector; description="Warning Received");

    @signal[beaconReceivedSignal](type=long);
    @statistic[beaconReceivedSignal](record=count,vector; description="Beacon Received");

    @signal[messageReceivedSignal](type=long);
    @statistic[messageReceivedSignal](record=count,vector; description="Message Received");


    @signal[newWarningReceivedSignal](type=long);
    @statistic[newWarningReceivedSignal](record=count,vector; description="New Warning Received");
}
