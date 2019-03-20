//
// Created by pu on 11/20/18.
//
#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"
#include "datatype.h"

#ifndef A2_ECE650_SOLVER_H
#define A2_ECE650_SOLVER_H


class Solver {
private:
    Minisat::Solver *solver;
    int number;
    int vertices;
    Minisat::Var **vars;
    Graph *graph;

public:
    std::list<int> getResult();
    bool isSatisficable();
    void everyEdgeIncident();
    void oneVertexPerPosition();
    void noTwiceVertex();
    void atLeastOneVertex();
    void init();
    Solver(int n, Graph *graph);
    ~Solver();

};


#endif //A2_ECE650_SOLVER_H
