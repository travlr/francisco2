package src.modules.application.flooding;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;

simple FloodingApplLayer extends BaseWaveApplLayer
{
    parameters:

        int accidentCount = default(1);

        @class(FloodingApplLayer);
        @display("i=block/app2");

        @signal[warningReceivedSignal](type=long);
        @statistic[warningReceivedSignal](record=count,vector; description="Warning Received");

        @signal[beaconReceivedSignal](type=long);
        @statistic[beaconReceivedSignal](record=count,vector; description="Beacon Received");

        @signal[messageReceivedSignal](type=long);
        @statistic[messageReceivedSignal](record=count,vector; description="Message Received");

        @signal[newWarningReceivedSignal](type=long);
        @statistic[newWarningReceivedSignal](record=count,vector; description="New Warning Received");
}
