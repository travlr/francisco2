#include "Junction.h"
#include <cxmlelement.h>
#include <cstdio>
using std::sscanf;


Junction::Junction(const std::string &id, const Coord &position)
    : id(id), position(position)
{
}

Junction::~Junction()
{

}
