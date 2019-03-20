//
// Created by pu on 11/20/18.
//

#include <iostream>
#include "Solver.h"

std::list<int> Solver::getResult() {
    auto result = std::list<int>();
    if (solver->solve()) {
        // satisfiable
        for (int v = 0; v < vertices; v++) {
            for (int k = 0; k < number; k++) {
                if (solver->modelValue(vars[v][k]) == Minisat::l_True) {
                    // victim found
                    result.push_back(v);
                }
            }
        }
        if (result.size() != number) {  // fall-safe TODO remove when completed
            std::cerr << "Error: *size does not match!" << std::endl;
        }
    }
    return result;  // size = 0 when invalid
}

bool Solver::isSatisficable() {
    return false;
}

void Solver::everyEdgeIncident() {
    for (int i = 0; i < vertices; i++) {
        for (int j = i + 1; j < vertices; j++) {
            if (graph->isConnected(i, j)) {
                // for every edge
                Minisat::vec<Minisat::Lit> lits;
                for (int k = 0; k < number; k++) {
                    lits.push(Minisat::mkLit(vars[i][k]));
                    lits.push(Minisat::mkLit(vars[j][k]));
                }
                solver->addClause(lits);

            }
        }
    }

}

void Solver::oneVertexPerPosition() {
    for(int m = 0; m < number; m++) {
        for(int p = 0; p < vertices; p++) {
            for(int q = p + 1; q < vertices; q++) {
                solver->addClause(~Minisat::mkLit(vars[p][m]), ~Minisat::mkLit(vars[q][m]));
            }
        }
    }

}

void Solver::noTwiceVertex() {
    for(int m = 0; m < vertices; m++) {
        for(int p = 0; p < number; p++) {
            for(int q = p + 1; q < number; q++) {
                solver->addClause(~Minisat::mkLit(vars[m][p]), ~Minisat::mkLit(vars[m][q]));
            }
        }
    }

}

void Solver::atLeastOneVertex() {
    for(int i = 0; i < number; i++) {
        Minisat::vec<Minisat::Lit> lits;
        for(int j = 0; j < vertices; j++) {
            lits.push(Minisat::mkLit(vars[j][i]));
        }
        solver->addClause(lits);
    }

}

void Solver::init() {
    atLeastOneVertex();
    noTwiceVertex();
    oneVertexPerPosition();
    everyEdgeIncident();
}

Solver::Solver(int n, Graph *graph) {
    this->graph = graph;
    solver = new Minisat::Solver();
    number = n;
    vertices = this->graph->getSize();
    int v = vertices;
    vars = new Minisat::Var* [v];
    for(int i =0; i < v; i++) {
        vars[i] = new Minisat::Var[n];
        for (int j = 0; j < n; j++) {
            vars[i][j] = solver->newVar();
        }
    }
}

Solver::~Solver() {
    for(int i=0; i < vertices; i++) {
        delete[] vars[i];
    }
    delete[] vars;
    delete solver;
}
