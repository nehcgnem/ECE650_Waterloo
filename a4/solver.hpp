// #pragma once

#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "minisat/mtl/Vec.h"


#include <string>
#include <sstream>
#include <iostream>
#include <vector>

// #include <vector>

// using board = std::vector<std::vector<int>>;

class Cover{

private:     
    Minisat::Solver solver;
    int n; // number of vertex 
    std::vector<int> edge;

    // might need sort edges ? to get number of actural vertices ?



public: 
    Cover(int n1, std::vector<int> edge_vect){
        int it = *max_element(begin(edge_vect), end(edge_vect)); // c++11
        if (it >= n1 ){  
            std::cerr << "Error: vertex out of range" << std::endl;
        }
        else {
            n = n1;
            edge = edge_vect;
        }
    }
    void solve();
};

/*
class Solver {
private:
    const bool m_write_dimacs = false;
    Minisat::Solver solver;

public:

    Solver(bool write_dimacs = false);
    // Returns true if applying the board does not lead to UNSAT result
    bool apply_board(board const&);
    // Returns true if the sudoku has a solution
    bool solve();
    board get_solution() const;

private:
    void one_square_one_value();
    void non_duplicated_values();
    void exactly_one_true(Minisat::vec<Minisat::Lit> const& literals);
    void init_variables();
};
*/