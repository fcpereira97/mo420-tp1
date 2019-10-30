#include <bits/stdc++.h>
#include "subgradient.h"
#include "preprocessing.h"
#include "mystruct.h"

using namespace std;

// Loads the number of vertices and edges
void load_graph_size(FILE* input_file, int *n_vertices, int *n_edges)
{
	int n = fscanf(input_file, "%d %d", n_vertices, n_edges);
}

// Loads the graph
void load_graph (FILE* input_file, int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	int v1, v2;

	for(int i = 0; i < n_vertices; i++)
	{
		(*vertices)[i] = new Vertex;
		
		(*vertices)[i]-> index = i;
		(*vertices)[i]-> degree = 0;
		(*vertices)[i]-> fixed = false;
	}

	for(int i = 0; i < n_edges; i++) 
	{
		int n = fscanf(input_file, "%d %d", &v1, &v2);
		(*edges)[i] = new Edge;

		(*edges)[i]-> index = i;
		(*edges)[i]-> vertex_1 = (*vertices)[v1-1];
		(*edges)[i]-> vertex_2 = (*vertices)[v2-1];
		(*edges)[i]-> fixed = false;
		(*vertices)[v1-1]-> degree++;
		(*vertices)[v2-1]-> degree++;
	}
}

// Main function
int main (int argc, char *argv[])
{
	// Time parameters
	time_t time_start;
  	time_start = time (NULL);

	// General variables
	string input_path, output_path;
	FILE *input_file;
	FILE *output_file;
	int n_vertices, n_edges;
	int time_limit;

	// Opens input and output files
	input_path = argv[1];
	output_path = argv[2];
	input_file = fopen(input_path.c_str(), "r");
	output_file = fopen(output_path.c_str(), "a");

	// Loads time limit
	time_limit = atoi(argv[3]);

	// Loads graph
	load_graph_size(input_file, &n_vertices, &n_edges);
	vector<Vertex*> vertices(n_vertices);
	vector<Edge*> edges(n_edges);
	load_graph(input_file, n_vertices, n_edges, &vertices, &edges);

	// Prints extra information
	
	/*
	FILE* extra_print;
	extra_print = fopen("results.csv", "a");
	fprintf(extra_print, "%s, ", input_path.c_str());
	fclose(extra_print);
	*/
	
	// Executes preprocessing algorithm
	preprocessing(n_vertices, n_edges, &vertices, &edges);

	// Executes subgradient algorithm
	subgradient(n_vertices, n_edges, &vertices, &edges, time_start, time_limit, output_file);

	// Closes input and output files
	fclose(input_file);
	fclose(output_file);


	return 0;
}