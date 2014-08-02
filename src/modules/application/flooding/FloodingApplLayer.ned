

package src.modules.application.flooding;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;

simple FloodingApplLayer extends BaseWaveApplLayer
{
    @class(FloodingApplLayer);
    @display("i=block/app2");

    @signal[warningReceived](type=long);
    @statistic[warningReceived](description="Warning Received"; record=stats,vector);
}
