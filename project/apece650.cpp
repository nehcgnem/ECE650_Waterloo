//
// Created by pu on 12/5/18.
//

#include <pthread.h>
#include <iostream>
#include <algorithm>
#include "PlainInterpreter.h"
#include "Solver.h"

pthread_t thdIo;
pthread_t thdSat;
pthread_t thdAprx1;
pthread_t thdAprx2;

std::string outputSat;
std::string outputAprx1;
std::string outputAprx2;

void* satThread(void* g) {
    auto graph = (Graph*)g;

    if (graph->edges.empty()) {  // no edge
        return nullptr;
    }

    int upper = graph->getSize(), lower = 1;
    std::list<int> result;

    do {  // avoid v = 1
        int k = (upper + lower) / 2;
        auto solver = new Solver(k, graph);
        solver->init();
        auto tResult = solver->getResult();
        // std::cerr << "Trying size of " << k << " U" << upper << " L" << lower << " S" << result.size() << std::endl;
        if (!tResult.empty()) {  // got victim
            upper = k - 1;
            result = tResult;
        } else { // no victim
            lower = k + 1;
        }
        delete solver;

    } while (upper >= lower);

    if (!result.empty()) {
        std::string m;
        for (auto victim : result) {
            outputSat += m;
            outputSat += std::to_string(victim);
            m = ",";
        }
    } else {
        outputSat = "";
    }


}

void* aprx1Thread(void* g) {
    auto graph = (Graph*)g;
    std::list<std::pair<int, int>> edges;
    edges.insert(std::end(edges), std::begin(graph->edges), std::end(graph->edges));  // duplicate
    std::list<int> result;

    while(!edges.empty()) {
        std::vector<int> vDegree(graph->getSize()+1);

        for(unsigned int i = 0; i < graph->getSize(); i++) {
            vDegree[i] = 0;
        }

        for (auto e : edges) {
            vDegree[e.first]++;
            vDegree[e.second]++;
        }

        // get max
        unsigned int maxIndex = 0;
        int max = vDegree[0];

        for(unsigned int i = 0; i < graph->getSize(); i++) {
            if (max < vDegree[i]) {
                max = vDegree[i];
                maxIndex = i;
            }
        }

        result.push_back(maxIndex);

        // remove edges incident
        auto i = edges.begin();
        while (i != edges.end()) {
            bool remove = (*i).first == maxIndex || (*i).second == maxIndex;
            if (remove)
            {
                edges.erase(i++);  // alternatively, i = items.erase(i);
            }
            else
            {
                ++i;
            }
        }

    }
    if (!result.empty()) {
        std::string m;
        for (auto victim : result) {
            outputAprx1 += m;
            outputAprx1 += std::to_string(victim);
            m = ",";
        }
    } else {
        outputAprx1 = "";
    }


}

void* aprx2Thread(void* g) {
    auto graph = (Graph*)g;
    std::list<std::pair<int, int>> edges;
    edges.insert(std::end(edges), std::begin(graph->edges), std::end(graph->edges));  // duplicate
    std::list<int> result;

    while (!edges.empty()) {
        auto discard = edges.front();
        result.push_back(discard.first);
        result.push_back(discard.second);

        // remove edges incident
        auto i = edges.begin();
        while (i != edges.end()) {
            bool remove = (*i).first == discard.first || (*i).second == discard.first;
            bool alsorm = (*i).first == discard.second || (*i).second == discard.second;
            if (remove || alsorm)
            {
                edges.erase(i++);  // alternatively, i = items.erase(i);
            }
            else
            {
                ++i;
            }
        }
    }
    if (!result.empty()) {
        std::string m;
        for (auto victim : result) {
            outputAprx2 += m;
            outputAprx2 += std::to_string(victim);
            m = ",";
        }
    } else {
        outputAprx2 = "";
    }

}

void* ioThread(void* unused) {

    Graph* graph;

    PlainInterpreter interpreter = PlainInterpreter();
    std::string input;

    while (std::cin) {
        std::getline(std::cin, input);
        try{
            graph = interpreter.call(input);
        }
        catch (char const* e) {
            std::cout<<e<<'\n';
            continue;
        }

        if (input[0] == 'E') {
            pthread_create(&thdSat, nullptr, &satThread, graph);
            pthread_create(&thdAprx1, nullptr, &aprx1Thread, graph);
            pthread_create(&thdAprx2, nullptr, &aprx2Thread, graph);

            pthread_join(thdSat, nullptr);
            pthread_join(thdAprx1, nullptr);
            pthread_join(thdAprx2, nullptr);

            std::cout << "CNF-SAT-VC: " << outputSat << std::endl;
            std::cout << "APPROX-VC-1: " << outputAprx1 << std::endl;
            std::cout << "APPROX-VC-2: " << outputAprx2 << std::endl;

            outputSat = "";
            outputAprx1 = "";
            outputAprx2 = "";
        }

    }


}



int main() {

    pthread_create(&thdIo, nullptr, &ioThread, nullptr);

    pthread_join(thdIo, nullptr);

    return 0;

}