#include <iostream>
#include <sstream>
#include "graph.hpp"
#include "solver.hpp"

// using namespace std;




int main(int argc, char **argv) {
   	std::string line; // the first line of the input sequence 
	std::string command; // store the operation command V or E
	std::string vertices; // store the string of vertices 
	std::string edges; // store the string of edges
	std::string start_vertex; // store the start point
	std::string end_vertex; // store the end point
	int n = 0; // store the number of vertices
	std::vector<int> adj[40000]; // adjency list representation of graph 
	std::vector<int> vect; //  store the int vertices  for building the graph 
	//graph 
	while (getline(std::cin, line))
	{

		//cout << line << std::endl;
		std::istringstream stream(line);
		stream >> command;
		if (command == "V"){ 
			stream >> vertices;
			if (vertices.empty()) 
			{
				std::cerr << "Error: Missing vertices" << std::endl;
				continue;}	
			n = stringtoInt(vertices); // initialize the graph 
			cleargraph(adj, n);
			vect.clear();
		}
		else if (command == "E"){
			if (n==0) 
			{
				std::cerr << "Error: Missing vertices" << std::endl; 
				continue;
			}
			stream >> edges;
			char const *charToRemove  = "{}<>";
			removeCharsFromString(edges, charToRemove);
			if (edges.empty()) 
			{
					// std::cerr << "Error: Missing edges" << std::endl;
				continue;
			}
			parseEdge(edges,vect);
			// adj = new Graph[n];
			buildgraph(adj,vect,n);

//Assignment 4 
				// for (std::vector<int>::const_iterator i = vect.begin(); i != vect.end(); ++i){
				// 	std::cout << *i << ' ';
				// }
				// std::cout << std::endl;
				//debug

			Cover s(n,vect);
			s.solve();



		}
		else if (command == "s") 
		{
			stream >> start_vertex >> end_vertex;
			if (start_vertex.empty() || end_vertex.empty())
			{
				std::cerr << "Error: missing starting or ending point" << std::endl;
				command = "";
			}
			else {
				printShortestDistance(adj, stringtoInt(start_vertex), stringtoInt(end_vertex), n); 
				command = "";
			}	 
		}
		else {
			std::cerr << "Error: invalid command" << std::endl;
		}


	}
	return 0;

}