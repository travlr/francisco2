
package src.modules.scenario;
import org.car2x.veins.nodes.Scenario;
import src.modules.stats.FranciscoStatistics;


network FranciscoScenario extends Scenario
{
    submodules:
        stats: FranciscoStatistics {
            @display("p=440,50");
        }
}

