//
// Created by pu on 2018/10/4.
//

#ifndef A2ECE650_DATATYPE_H
#define A2ECE650_DATATYPE_H

#include  <list>

class GraphCalculation;

class Graph  {
private:
    GraphCalculation* graphCalculation = nullptr;
    int** connectivity = nullptr;
    unsigned int size = 0;
    bool isBoundary(int) const;


public:
    std::list<std::pair<int,int>> edges;
    explicit Graph(int size);
    ~Graph();
    void bindCalculationMethod(GraphCalculation*);
    std::list<int> *getCriticalPath(int src, int dst) const;
    inline unsigned int getSize() const;
    void setConnection(int, int);
    void setConnection(std::pair<int, int>);

    inline bool isConnected(int, int) const ;

};

inline bool Graph::isConnected(int i0, int i1) const {
    return this->connectivity[i0][i1] >= 1;
}

inline unsigned int Graph::getSize() const {
    return size;
}

class GraphCalculation
{
public:
    virtual std::list<int>* getCritical(const Graph*, int, int) = 0;
    // ~GraphCalculation() = default;
};

#endif //A2ECE650_DATATYPE_H
