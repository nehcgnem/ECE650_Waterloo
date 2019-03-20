#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "stdc++.h"
// using namespace std; 

void add_edge(std::vector<int> adj[], int src, int dest);

void removeCharsFromString( std::string &str, const char* charsToRemove );

void parseEdge(std::string s, std::vector<int> & v);

int stringtoInt(std::string s);

void buildgraph(std::vector<int> adj[], std::vector<int> & v, int n);

bool BFS(std::vector<int> adj[], int src, int dest, int v, 
	int pred[], int dist[]); 

void printShortestDistance(std::vector<int> adj[], int s, 
	int dest, int v);

void cleargraph(std::vector<int> adj[], int n );



#endif