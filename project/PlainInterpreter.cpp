//
// Created by pu on 2018/10/9.
//

#include <iostream>
#include "PlainInterpreter.h"
#include "EIterator.h"
#include "IJK.h"
#include "Solver.h"

void PlainInterpreter::eCommand(std::string command) {
    if (!this->graph) {
        throw "Error: Please specify graph size first.";
    }
    // std::list<std::pair<int,int>> parsed;
    try{
        EIterator eIterator = EIterator(command);
        while (eIterator.hasNext()) {
            graph->edges.push_back(eIterator.getNext());  // local variable used to discard all input on error
        }

        for (auto entry : graph->edges) {
            this->graph->setConnection(entry);
        }

    } catch (char const* e) {
        delete this->graph;
        this->graph = nullptr;
        throw e;
    }

//    if (graph->edges.empty()) {  // no edge
//        std::cout << std::endl;
//        return;
//    }
//
//    int upper = graph->getSize(), lower = 1;
//    std::list<int> result;
//
//    do {  // avoid v = 1
//        int k = (upper + lower) / 2;
//        auto solver = new Solver(k, graph);
//        solver->init();
//        auto tResult = solver->getResult();
//        // std::cerr << "Trying size of " << k << " U" << upper << " L" << lower << " S" << result.size() << std::endl;
//        if (!tResult.empty()) {  // got victim
//            upper = k - 1;
//            result = tResult;
//        } else { // no victim
//            lower = k + 1;
//        }
//        delete solver;
//
//    } while (upper >= lower);
//
//    if (!result.empty()) {
//        std::string m;
//        for (auto victim : result) {
//            std::cout << m << victim;
//            m = " ";
//        }
//        std::cout << std::endl;
//    }


//    for (int k = 1; k <= this->graph->getSize(); k++) {
//        auto solver = new Solver(k, graph);
//        solver->init();
//        auto result = solver->getResult();
//        if (!result.empty()) {
//            std::string m;
//            for (auto victim : result) {
//                std::cout << m << victim;
//                m = " ";
//            }
//            delete solver;
//            break;
//        }
//        delete solver;
//    }
}

void PlainInterpreter::vCommand(std::string command) {
    int size = stoi(command);
    delete this->graph;
    this->graph = new Graph(size);
    this->graph->bindCalculationMethod(new IJK());
}

void PlainInterpreter::sCommand(std::string command) {
    if (!this->graph) {
        throw "Error: Please specify graph size first.";
    }
    unsigned int idx = command.find(' ');
    int from = stoi(command);
    int to = stoi(command.substr(idx + 1));
    auto result = graph->getCriticalPath(from, to);
    if (result == nullptr) {
        throw "Error: Path not exist.";
    } else {
        std::string sp;
        for (auto r : *result) {
            std::cout<<sp<<r;
            sp = "-";
        }
        std::cout<<'\n';
    }


}

PlainInterpreter::PlainInterpreter() {
    this->graph = nullptr;

}

PlainInterpreter::~PlainInterpreter() {
    delete this->graph;
}

Graph* PlainInterpreter::call(std::string command) {
    char c = command[0];
    command.erase(0,2);

    switch (c) {
        case 'V':
            vCommand(command);
            break;
        case 'E':
            eCommand(command);
            break;
        case 's':
            sCommand(command);
            break;
        case '\0':  // EOF
            break;
        default:
            throw "Error: Unknown command type.";

    }

    return graph;

}
