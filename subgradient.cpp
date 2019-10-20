#include <bits/stdc++.h>
#include "subgradient.h"
#include "agmr_rl1.h"
#include "agmr_rl2.h"

using namespace std;

// Sets the initial values of lambdas variables
void set_initial_lambdas(int n_vertices, vector<Vertex*> *vertices)
{
	int min_degree = n_vertices;

	for (int i = 0; i < n_vertices; ++i)
	{
		min_degree = min(min_degree, (*vertices)[i]->degree);
	}

	for (int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> degree <= 2)
		{
			(*vertices)[i]-> lambda = 0;
		}
		else
		{
			(*vertices)[i]-> lambda = 1.0 / (double) min_degree;
		}
	}
}

// Sets the values of lambdas variables
void set_lambdas(int n_vertices, double step_size, vector<Vertex*> *vertices, vector<int> *subgradient_vector)
{
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> degree > 2)
		{
			double cost = (*vertices)[i]-> lambda + step_size * (*subgradient_vector)[i];
			if(cost < 0)
			{
				(*vertices)[i]-> lambda = 0;
			}
			else if (cost >= 0 && cost <= 1.0/(double)(*vertices)[i]-> degree)
			{
				(*vertices)[i]-> lambda = cost;
			}
			else
			{
				//cout << "here" << endl;
				(*vertices)[i]-> lambda = 1.0/(double)(*vertices)[i]-> degree;

				//cout << (double)((*vertices_degrees)[i] * (*lambdas)[i]) << endl;
			}
		}
	}
}

double get_rl_primal_value(int n_vertices, vector<Vertex*> *vertices, double rl1_value, double rl2_value)
{
	double current_lower_bound = 0;
	for (int i = 0; i < n_vertices; i++)
	{
		current_lower_bound += (*vertices)[i]-> lambda;
	}
	current_lower_bound = (-2) * current_lower_bound;
	current_lower_bound += rl1_value + rl2_value;

	return current_lower_bound;
}

void calc_subgradient(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges,
	vector<int> *subgradient, vector<bool> *edges_variables, vector<bool> *vertices_variables)
{
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> degree > 2)
		{
			(*subgradient)[i] += - 2 - (*vertices)[i]-> degree * (*vertices_variables)[i];
		}
	}

	for(int i = 0; i < n_edges; i++)
	{
		Vertex *v1, *v2;

		v1 = (*edges)[i]-> vertex_1;
		v2 = (*edges)[i]-> vertex_2;

		if(v1->degree > 2)
		{
			(*subgradient)[v1-> index] += (*edges_variables)[i];
		}

		if(v1->degree > 2)
		{
			(*subgradient)[v2-> index] += (*edges_variables)[i];
		}
	}
}

double calc_step_size(int n_vertices, double current_lower_bound, vector<int> *subgradient_vector)
{
	double epsilon, numerator, denominator, step_size;

	epsilon = 0.1;
	numerator = n_vertices - current_lower_bound;

	denominator = 0;
	for(int i = 0; i < n_vertices; i++)
	{
		denominator += pow((*subgradient_vector)[i], 2);
	}

	step_size = epsilon * (numerator/denominator);
	return step_size;
}


void subgradient(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	// Vector of lambdas of langrarian relaxation

	vector<int> subgradient_vector(n_vertices);
	vector<bool> edges_variables(n_edges, false);
	vector<bool> vertices_variables(n_vertices, false);
	int iteration, current_upper_bound, best_upper_bound;
	double rl1_value, rl2_value, current_lower_bound, best_lower_bound, step_size;
	
	set_initial_lambdas(n_vertices, vertices);
	best_lower_bound = numeric_limits<double>::min();
	best_upper_bound = numeric_limits<int>::max();
	
	
	for(int iteration = 0; iteration < 10; iteration++)
	{

		// Executes Kruskal algorithm for getting a minimum spanning tree for RL1 problem
		fill(edges_variables.begin(), edges_variables.end(), false);
		rl1_value = kruskal(n_vertices, n_edges, vertices, edges, &edges_variables);
		
		// Executes inspection algorithm for RL2 problem
		fill(vertices_variables.begin(), vertices_variables.end(), false);
		rl2_value = inspection(n_vertices, vertices, &vertices_variables);
		
		// Calculates RL solution value
		current_lower_bound = get_rl_primal_value(n_vertices, vertices, rl1_value, rl2_value);
		
		fill(subgradient_vector.begin(), subgradient_vector.end(), 0);
		calc_subgradient(n_vertices, n_edges, vertices, edges, &subgradient_vector, &edges_variables, &vertices_variables);
		
		step_size = calc_step_size(n_vertices, current_lower_bound, &subgradient_vector);
		
		set_lambdas(n_vertices, step_size, vertices, &subgradient_vector);

		best_lower_bound = max(best_lower_bound, current_lower_bound);
		
		cout << "Iteration " << iteration + 1 << ": " << rl1_value << " " << rl2_value << " " << current_lower_bound << endl;
		
	}

	cout << "Best LB: " << best_lower_bound << endl;
	cout << "Best UB: " << best_upper_bound << endl;

}