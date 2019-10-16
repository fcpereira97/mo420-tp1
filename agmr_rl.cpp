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
void set_initial_lambdas(int n_vertices, vector<int> *vertices_degrees, vector<double> *vertices_lambdas)
{
	int min_degree = n_vertices;

	for (int i = 0; i < n_vertices; ++i)
	{
		min_degree = min(min_degree, (*vertices_degrees)[i]);
	}

	for (int i = 0; i < n_vertices; i++)
	{
		if((*vertices_degrees)[i] <= 2)
		{
			(*vertices_lambdas)[i] = 0;
		}
		else
		{
			(*vertices_lambdas)[i] = 1.0 / (double) min_degree;
		}
	}
}

// Sets the values of lambdas variables
void set_lambdas(int n_vertices, double step_size, vector<int> *vertices_degrees, vector<double> *vertices_lambdas,
	vector<double> *subgradients)
{
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices_degrees)[i] > 2)
		{
			double cost = (*vertices_lambdas)[i] + step_size * (*subgradients)[i];
			if(cost < 0)
			{
				(*vertices_lambdas)[i] = 0;
			}
			else if (cost >= 0 && cost <= 1.0/(double)(*vertices_degrees)[i])
			{
				(*vertices_lambdas)[i] = cost;
			}
			else
			{
				(*vertices_lambdas)[i] = 1.0/(double)(*vertices_degrees)[i];
			}
		}
	}
}

double get_rl_primal_value(int n_vertices, double rl1_sol_value, double rl2_sol_value, vector<double> *vertices_lambdas)
{
	double rl_sol_value = 0;
	for (int i = 0; i < n_vertices; i++)
	{
		rl_sol_value += (*vertices_lambdas)[i];
	}
	rl_sol_value = (-2) * rl_sol_value;
	rl_sol_value += rl1_sol_value + rl2_sol_value;

	return rl_sol_value;
}

void calc_subgradients(int n_vertices, int n_edges, vector<int> *vertices_degrees, vector<pair<int,int>> *edges,
	vector<double> *subgradients, vector<bool> *edges_variables, vector<bool> *vertices_variables)
{
	
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices_degrees)[i] > 2)
			(*subgradients)[i] += (-2) - ((*vertices_degrees)[i] * (*vertices_variables)[i]);
	}

	for(int i = 0; i < n_edges; i++)
	{
		int v1, v2;

		v1 = (*edges)[i].first;
		v2 = (*edges)[i].second;

		if((*vertices_degrees)[v1] > 2)
			(*subgradients)[v1] += (*edges_variables)[i];

		if((*vertices_degrees)[v2] > 2)
			(*subgradients)[v2] += (*edges_variables)[i];
	}
}

double calc_step_size(int n_vertices, double rl_sol_value,
	vector<double> *vertices_lambdas, vector<double> *subgradients)
{
	double epsilon, numerator, denominator, step_size;

	epsilon = 0.1;
	numerator = 15 - rl_sol_value;

	denominator = 0;
	for(int i = 0; i < n_vertices; i++)
	{
		denominator += pow((*subgradients)[i], 2);
	}

	step_size = epsilon * (numerator/denominator);

	//cout << step_size << endl;
	return step_size;
}

void subgradient(int n_vertices, int n_edges, vector<int> *vertices_degrees, vector<pair<int,int>> *edges)
{
	// Vector of lambdas of langrarian relaxation
	vector<double> vertices_lambdas(n_vertices);
	vector<double> subgradients(n_vertices);
	vector<bool> edges_variables(n_edges, false);
	vector<bool> vertices_variables(n_vertices, false);
	int iteration;
	double rl1_sol_value, rl2_sol_value, rl_sol_value, best_sol_value, step_size;

	set_initial_lambdas(n_vertices, vertices_degrees, &vertices_lambdas);
	best_sol_value = -1;

	for(int iteration = 0; iteration < 100; iteration++)
	{
		// Executes Kruskal algorithm for getting a minimum spanning tree for RL1 problem
		fill(edges_variables.begin(), edges_variables.end(), false);
		rl1_sol_value = kruskal(n_vertices, n_edges, vertices_degrees, edges, &vertices_lambdas, &edges_variables);

		// Executes inspection algorithm for RL2 problem
		fill(vertices_variables.begin(), vertices_variables.end(), false);
		rl2_sol_value = inspection(n_vertices, vertices_degrees, &vertices_lambdas, &vertices_variables);

		// Calculates RL solution value
		rl_sol_value = get_rl_primal_value(n_vertices, rl1_sol_value, rl2_sol_value, &vertices_lambdas);

		fill(subgradients.begin(), subgradients.end(), 0);
		calc_subgradients(n_vertices, n_edges, vertices_degrees, edges, &subgradients, &edges_variables, &vertices_variables);

		step_size = calc_step_size(n_vertices, rl_sol_value, &vertices_lambdas, &subgradients);

		set_lambdas(n_vertices, step_size, vertices_degrees, &vertices_lambdas, &subgradients);

		best_sol_value = max(best_sol_value, rl_sol_value);

		cout << "Iteração " << iteration + 1 << ": " << rl1_sol_value << " " << rl2_sol_value << " " << rl_sol_value << endl;

	}
	cout << "Best LB: " << best_sol_value << endl;
}

int main (int argc, char *argv[])
{
	// General variables
	string input_path, output_path;
	FILE *input_file;
	FILE *output_file;
	int n_vertices, n_edges;

	// Opens input and output files
	input_path = argv[1];
	//output_path = argv[2];
	input_file = fopen(input_path.c_str(), "r");
	//output_file = fopen(output_path.c_str(), "a");

	// Loads graph
	load_graph_size(input_file, &n_vertices, &n_edges);
	vector<pair<int,int>> edges(n_edges); // Array of edges represented by its extremities
	vector<int> vertices_degrees (n_vertices, 0); // Array vertices degrees
	load_edges(input_file, n_edges, &edges, &vertices_degrees);

	//Executes subgradient algorithm
	subgradient(n_vertices, n_edges, &vertices_degrees, &edges);

	// Closes input and output files
	fclose(input_file);
	//fclose(output_file);

	return 0;
}