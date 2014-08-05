package src.modules.obstacles;


simple RavObstacleControl
{
    parameters:
        bool debug = default(false);  // emit debug messages?
        xml obstacles = default(xml("<obstacles/>")); // obstacles to add at startup
        @class(RavObstacleControl);
        @display("i=misc/town");
        @labels(node);
}
