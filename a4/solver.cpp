#include "solver.hpp"
#include <iostream>
using Minisat::mkLit;
using Minisat::lbool;

void Cover::solve(){
   //  for (std::vector<int>::const_iterator i = edge.begin(); i != edge.end(); ++i){
   //     std::cout << *i << ' ';
   // }
   // std::cout << std::endl;
    
    for (int k = 1 ; k < n; ++k){ // loop on k [1,n]
        // std::cout <<"k_main: "<< k << std::endl;
        Minisat::Solver solver;
        Minisat::Lit x[n][k];
// setup lit matrix  i [1,n],  j [1,k]

        for(unsigned int j = 0; j < k; j++){
            for (unsigned int i = 0; i < n; i ++){
                x[i][j] = Minisat::mkLit(solver.newVar());

            }
        }
// first clause
        Minisat::vec<Minisat::Lit> ps;
        for (unsigned int i = 0; i <k; i++){
            for (unsigned int j = 0; j < n; j++){
                ps.push(x[j][i]);
                // solver.addClause(x[j][i]);
                // solver.addClause(x[j][i], x[j][i]);
            }
        }
        solver.addClause(ps);

// second clause 
        for (unsigned int m = 0; m < n; m++){
            for (unsigned int p = 0; p < k; p++ ){
                for (unsigned int q = p+1; q < k; q++){
                    solver.addClause(~x[m][p], ~x[m][q]);
                }
            }
        }
// third clause
        for (unsigned int m =0 ; m < k; m++){
            for (unsigned int p = 0; p < n; p ++){
                for (unsigned int q = p +1; q < n; q ++ ){
                    solver.addClause(~x[p][m], ~x[q][m]);
                }
            }
        }
// forth clause 
        for (std::vector<int>::const_iterator i = edge.begin(); i != edge.end(); ++i){
            Minisat::vec<Minisat::Lit> ps;
            int v1 = *i;
            std::advance (i,1);
            int v2 = *i;
            for (unsigned int j = 0; j<k; j++){
                ps.push(x[v1][j]);
                ps.push(x[v2][j]);
                // std::cout << "k4:"<<j << std::endl;
                // std::cout << v1 << ' ';
                // std::cout << v2 << ' ';
                // std::cout << std::endl;
                // std::cout << "k:"<<j << std::endl;
            }

            solver.addClause(ps);
        }
//result 
        int t = 0;
        if (solver.solve()){
            for (unsigned int i =0; i < n ; i++){
                for (unsigned int j = 0; j < k; j++){
                    if(solver.modelValue(x[i][j])==Minisat::l_True){
                       // push back i 
                        std::cout <<t<<" ";
                    }
                }
                t++;
            }
            std::cout << std::endl;
            break;
        }
        // else if(solver.solve()==false){
        //     std::cout << std::endl;
        // }
    }
}



