//
// Created by pu on 2018/10/9.
//

#ifndef A2ECE650_PLAININTERPRETER_H
#define A2ECE650_PLAININTERPRETER_H

#include <string>

#include "datatype.h"

class PlainInterpreter {
private:
    Graph *graph;
    void eCommand(std::string);
    void vCommand(std::string);
    void sCommand(std::string);


public:
    PlainInterpreter();
    ~PlainInterpreter();

    Graph* call(std::string) noexcept(false);


};


#endif //A2ECE650_PLAININTERPRETER_H
