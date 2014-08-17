package src.modules.obstacles;


simple RavObstacleControl
{
    parameters:

        bool debug = default(false);  // emit debug messages?
        xml obstacles = default(xml("<obstacles/>")); // obstacles to add at startup

        double streetAngularDifferenceThreshold = default(20);
        double distanceFromVehicleToStreetProjectionThreshold = default(20);
        double distanceOfVehicleFromJunctionThreshold = default(20);

        @class(RavObstacleControl);
        @display("i=misc/town");
        @labels(node);
}
