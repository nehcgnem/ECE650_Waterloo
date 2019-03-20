//
// Created by pu on 2018/10/5.
//

#include "datatype.h"
#include <list>

Graph::Graph(int size) {
    this->size = size;
    this->connectivity = new int*[size];
    for (int i = 0; i < size; i++) {
        this->connectivity[i] = new int[size]();
    }
}

Graph::~Graph() {
    for (int i = 0; i < size; i++) {
        delete[] this->connectivity[i];
    }
    delete[] this->connectivity;
}

void Graph::bindCalculationMethod(GraphCalculation *gc) {
    this->graphCalculation = gc;
}

std::list<int> *Graph::getCriticalPath(int src, int dst) const {
    if (!isBoundary(src) || (!isBoundary(dst)) ) {
        throw "Error: Index out of range.";
    }
    if (dst == src) {
        auto result = new std::list<int>;
        result->push_back(src);
        return result;
    }
    return this->graphCalculation->getCritical(this, src, dst);
}

void Graph::setConnection(int i0, int i1) {
    if (!isBoundary(i0) || (!isBoundary(i1)) ) {
        throw "Error: Index out of range.";
    }
    if (i0 == i1) {
        throw "Error: Self loop.";
    }
    this->connectivity[i0][i1] = 1;
    this->connectivity[i1][i0] = 1;
}

void Graph::setConnection(std::pair<int, int> pair) {
    this->setConnection(pair.first, pair.second);

}

bool Graph::isBoundary(int index) const {
    return (index >= 0) && (index < this->size);
}

