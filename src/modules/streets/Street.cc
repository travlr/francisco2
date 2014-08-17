#include "Street.h"
#include <cstring>
#include <vector>
#include <cstdlib>

using std::string;
using std::vector;

Street::Street(const string & traciId)
    : traciId(traciId)
{
//    cXMLAttributeMap attrMap = streetEl->getAttributes();
//    xmlId = attrMap["id"];
//    from = attrMap["from"];
//    to = attrMap["to"];
//    priority = attrMap["priority"];
//    type = attrMap["type"];
//    xmlShape = attrMap["shape"];


//    // get lanes
//    cXMLElementList laneEls = streetEl->getChildrenByTagName("lane");

//    for (uint i = 0; i < laneEls.size(); ++i) {
//        cXMLElement* el = laneEls[i];
//        Lane* l = new Lane;
//        cXMLAttributeMap m = el->getAttributes();
//        l->xmlId = m["id"];
//        l->index = m["index"];
//        l->length = m["length"];
//        l->xmlShape = m["shape"];
//        if (i == 0 && shape.empty())
//            xmlShape = l->xmlShape;
//        l->speed = m["speed"];
//        lanes.push_back(l);
//    }
//    // end get lanes


//    // street segments
//    vector<string> segmentStrings;
//    char* tok;
//    tok = strtok((char*)shape.c_str(), " ");

//    while (tok != NULL) {
//        segmentStrings.push_back(tok);
//        tok = strtok(NULL, " ");
//    }

//    vector<double> coordFields;

//    size_t numSegmentStrings = segmentStrings.size();

//    for (uint i = 0; i < segmentStrings.size(); ++i) {
//        tok = strtok((char*)segmentStrings[i].c_str(), ",");
//        while (tok != NULL) {
//            coordFields.push_back(atof(tok));
//            tok = strtok(NULL, ",");
//        }
//    }

//    size_t numCoordFields =  coordFields.size();

//    for (uint i = 0; i < coordFields.size()-3; i+=4) {
//        StreetSegment* ss = new StreetSegment;
//        ss->startPosition = Coord(coordFields[i], coordFields[i+1]);
//        ss->endPosition   = Coord(coordFields[i+2], coordFields[i+3]);
//        streetSegments.push_back(ss);
//    }
//    // end street segments
}

Street::~Street()
{
    for (uint i = 0; i < lanes.size(); ++i) {
        delete lanes[i];
    }
    for (uint i = 0; i < streetSegments.size(); ++i) {
        delete streetSegments[i];
    }
}



