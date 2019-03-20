//
// The famed dIJKstra Algorithm.
// Created by pu on 2018/10/4.
//

#include <stack>
#include <cstring>
#include <list>

#include "IJK.h"
#include "datatype.h"


std::list<int> *IJK::getCritical(const Graph * graph, int src, int dst) {

    int *previous = new int[graph->getSize()]();  // nearest vertex
    int *distance = new int[graph->getSize()]();

    int *isVisited = new int[graph->getSize()]();

    int min, minIndex = src;

    int infinity = graph->getSize() + 1;  // Max path length in a connected graph is the number of vertices.

    // init
    for (int i = 0; i < graph->getSize(); i++) {
        previous[i] = graph->isConnected(src, i) ? src : -1;
        distance[i] = graph->isConnected(src, i) ? 1 : infinity;
    }
    isVisited[src] = 1;
    distance[src] = 0;


    // Dijkstra
    for (int i = 0; i < graph->getSize(); i++) {
        min = infinity;
        for (int j = 0; j < graph->getSize(); j++) {
            if (!isVisited[j] && distance[j] < min) {
                min = distance[j];
                minIndex = j;
            }
        }
        if (min >= infinity) {
            break;
        }
        isVisited[minIndex] = 1;
        for (int j =0; j < graph->getSize(); j++) {
            if (distance[j] > min + (graph->isConnected(minIndex, j) ? 1 : infinity)) {
                previous[j] = minIndex;
                distance[j] = min + (graph->isConnected(minIndex, j) ? 1 : infinity);
            }
        }
    }

    // build path
    if (distance[dst] >= infinity) {  //
        delete[] previous;
        delete[] distance;
        delete[] isVisited;
        return nullptr;
    }
    std::stack<int> pathStack;
    for (int i = dst; previous[i] !=src; i = previous[i]) {
        pathStack.push(previous[i]);
    }

    auto *fullPath = new std::list<int>;
    fullPath->push_back(src);
    int stackSize = pathStack.size();
    for (int i = 1; i < stackSize + 1; i++) {
        fullPath->push_back(pathStack.top());
        pathStack.pop();
    }
    fullPath->push_back(dst);

    // cleanup & return
    delete[] previous;
    delete[] distance;
    delete[] isVisited;
    return fullPath;
}

