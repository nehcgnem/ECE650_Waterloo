//
// The famed dIJKstra Algorithm.
// Created by pu on 2018/10/4.
//

#ifndef A2ECE650_IJK_H
#define A2ECE650_IJK_H

#include <list>

#include "datatype.h"

class IJK: public GraphCalculation {
public:
    std::list<int> *getCritical(const Graph *graph, int i, int i1) override;
};


#endif //A2ECE650_IJK_H
