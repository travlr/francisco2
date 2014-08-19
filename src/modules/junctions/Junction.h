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
    Junction(const string & id, const Coord & position);
    ~Junction();

    string getId() const { return id; }
    Coord getPosition() const { return position; }

protected:
    string id;
    Coord position;
//    string incLanes;
//    list<Request> requests;
};

//struct Request
//{
//    string index;
//    string response;
//    string foes;
//    string cont;
//};

#endif // JUNCTION_H
