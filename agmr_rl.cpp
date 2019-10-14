#include <bits/stdc++.h>
#include "agmr_rl1.h"
#include "agmr_rl2.h"

using namespace std;

// Loads the number of vertices and edges
void load_graph_size(FILE* input_file, int *n_vertices, int *n_edges)
{
	fscanf(input_file, "%d %d", n_vertices, n_edges);
}

// Loads the edges and calculate vertices' degrees
void load_edges(FILE* input_file, int n_edges, vector<pair<int,int>> *edges, vector<int> *vertices_degrees)
{
	int v1, v2;
	for(int i = 0; i < n_edges; i++) 
	{
		fscanf(input_file, "%d %d", &v1, &v2);
		(*edges)[i] = make_pair(v1 - 1, v2 - 1);	// Save edges
		(*vertices_degrees)[v1-1]++;	// Update degrees
		(*vertices_degrees)[v2-1]++; 
	}
}

// Sets the initial values of lambdas variables
void set_lambdas(int n_vertices, vector<int> *vertices_degrees, vector<double> *vertices_lambdas)
{
	int max_degree = 0;

	for (int i = 0; i < n_vertices; ++i)
	{
		max_degree = max(max_degree, (*vertices_degrees)[i]);
	}

	for (int i = 0; i < n_vertices; i++)
	{
		if((*vertices_degrees)[i] <= 2)
		{
			(*vertices_lambdas)[i] = 0;
		}
		else
		{
			(*vertices_lambdas)[i] = 1.0 / (double) max_degree;
		}
	}
}

int main (int argc, char *argv[])
{
	// General variables
	string input_path, output_path;
	FILE *input_file;
	FILE *output_file;
	int n_vertices, n_edges;
	double rl1_sol_value, rl2_sol_value, rl_sol_value;

	// Opens input and output files
	input_path = argv[1];
	output_path = argv[2];
	input_file = fopen(input_path.c_str(), "r");
	output_file = fopen(output_path.c_str(), "a");

	// Loads graph
	load_graph_size(input_file, &n_vertices, &n_edges);
	vector<pair<int,int>> edges(n_edges); // Array of edges represented by its extremities
	vector<int> vertices_degrees (n_vertices, 0); // Array vertices degrees
	load_edges(input_file, n_edges, &edges, &vertices_degrees);

	// Vector of lambdas of langrarian relaxation
	vector<double> vertices_lambdas(n_vertices);
	set_lambdas(n_vertices, &vertices_degrees, &vertices_lambdas);

	// Executes Kruskal algorithm for getting a minimum spanning tree for RL1 problem
	rl1_sol_value = kruskal(n_vertices, n_edges, &edges, &vertices_lambdas);

	// Executes inspection algorithm for RL2 problem
	rl2_sol_value = inspection(n_vertices, &vertices_degrees, &vertices_lambdas);

	// Calculates RL solution value
	rl_sol_value = 0;
	for (int i = 0; i < n_vertices; i++)
	{
		rl_sol_value += vertices_lambdas[i];
	}
	rl_sol_value = (-2) * rl_sol_value;
	rl_sol_value += rl1_sol_value + rl2_sol_value;

	cout << rl1_sol_value << " " << rl2_sol_value << " " << rl_sol_value << endl;

	// Closes input and output files
	fclose(input_file);
	fclose(output_file);

	return 0;
}