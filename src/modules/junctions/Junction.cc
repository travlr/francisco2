#include "Junction.h"
#include <cxmlelement.h>
#include <cstdio>
using std::sscanf;


Junction::Junction(cXMLElement* junctionEl)
    : element(junctionEl)
{
    cXMLAttributeMap m = junctionEl->getAttributes();
    id = m["id"];
    type = m["type"];
    double x = -1;
    double y = -1;
    sscanf(m["x"].c_str(), "%lf", &x);
    ASSERT(x != -1);
    sscanf(m["y"].c_str(), "%lf", &y);
    ASSERT(y != -1);
    position = new Coord(x,y);
}

Junction::~Junction()
{

}
