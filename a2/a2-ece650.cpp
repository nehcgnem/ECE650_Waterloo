#include <iostream>
#include <sstream>
#include "graph.hpp"
using namespace std;

// class Graph
// {
// private:
//     vector<int> * adj;

// // public:
// //     Date(int year, int month, int day);

// //     void SetDate(int year, int month, int day);

// //     int getYear() { return m_year; }
// //     int getMonth() { return m_month; }
// //     int getDay()  { return m_day; }
// };



int main(){
   	string line; // the first line of the input sequence 
	string command; // store the operation command V or E
	string vertices; // store the string of vertices 
	string edges; // store the string of edges
	string start_vertex; // store the start point
	string end_vertex; // store the end point
	int n = 0; // store the number of vertices
	vector<int> adj[40000]; // adjency list representation of graph 
	vector<int> vect; //  store the int vertices for building the graph 


	//graph 
	while (getline(cin, line))
	{

		//cout << line << endl;
		istringstream stream(line);
		stream >> command;
		if (command == "V"){ 
			stream >> vertices;
			if (vertices.empty()) 
			{
				cerr << "Error: Missing vertices" << endl;
				continue;}	
			n = stringtoInt(vertices); // initialize the graph 
			cleargraph(adj, n);
			vect.clear();
		}
		else if (command == "E"){
			if (n==0) 
			{
				cerr << "Error: Missing vertices" << endl; 
				continue;
			}
				stream >> edges;
				char const *charToRemove  = "{}<>";
				removeCharsFromString(edges, charToRemove);
				if (edges.empty()) 
				{
					cerr << "Error: Missing edges" << endl;
					continue;
				}
				parseEdge(edges,vect);
			// adj = new Graph[n];
				buildgraph(adj,vect,n);
			}
			else if (command == "s") 
			{
				stream >> start_vertex >> end_vertex;
				if (start_vertex.empty() || end_vertex.empty())
				{
					cerr << "Error: missing starting or ending point" << endl;
					command = "";
				}
				else {
					printShortestDistance(adj, stringtoInt(start_vertex), stringtoInt(end_vertex), n); 
					command = "";
				}	 
			}
			else {
				cerr << "Error: invalid command" << endl;
			}


		}

	}
