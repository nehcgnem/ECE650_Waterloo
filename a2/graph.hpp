#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "stdc++.h"
using namespace std; 

void add_edge(vector<int> adj[], int src, int dest);

void removeCharsFromString( string &str, const char* charsToRemove );

void parseEdge(string s, vector<int> & v);

int stringtoInt(string s);

void buildgraph(vector<int> adj[], vector<int> & v, int n);

bool BFS(vector<int> adj[], int src, int dest, int v, 
	int pred[], int dist[]); 

void printShortestDistance(vector<int> adj[], int s, 
	int dest, int v);

void cleargraph(vector<int> adj[], int n );



#endif