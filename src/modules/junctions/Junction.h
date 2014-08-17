#ifndef JUNCTION_H
#define JUNCTION_H

#include <string>
using std::string;
#include <list>
using std::list;
#include <Coord.h>

class Request;

class Junction
{

public:
    Junction(cXMLElement* junctionEl);
    ~Junction();

    string getId() const { return id; }
    string getType() const { return type; }
    Coord* getPosition() const { return position; }

protected:
    cXMLElement* element;
    string id;
    string type;
    Coord* position;
    string incLanes;
    list<Request> requests;
};

struct Request
{
    string index;
    string response;
    string foes;
    string cont;
};

#endif // JUNCTION_H
