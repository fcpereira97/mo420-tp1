#include <bits/stdc++.h>
#include "subgradient.h"
#include "agmr_rl1.h"
#include "agmr_rl2.h"
#include "heuristic.h"

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

int calc_sol_value(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	int sol_val = 0;

	for(int i = 0; i < n_vertices; i++)
		(*vertices)[i]-> ramifications = 0;

	for(int i = 0; i < n_edges; i++)
	{
		if((*edges)[i]-> variable)
		{
			Vertex *v1, *v2;
			v1 = (*edges)[i]-> vertex_1;
			v2 = (*edges)[i]-> vertex_2;

			v1-> ramifications++;
			v2-> ramifications++;
		}
	}

	for(int i = 0; i < n_vertices; i++)
		if((*vertices)[i]-> ramifications > 2){
			sol_val++;
		}

	return sol_val;
}

double get_lower_bound(int n_vertices, vector<Vertex*> *vertices, double rl1_value, double rl2_value)
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
	vector<int> *subgradient)
{
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> degree > 2)
		{
			(*subgradient)[i] += - 2 - (*vertices)[i]-> degree * (*vertices)[i]-> variable;
		}
	}

	for(int i = 0; i < n_edges; i++)
	{
		Vertex *v1, *v2;

		v1 = (*edges)[i]-> vertex_1;
		v2 = (*edges)[i]-> vertex_2;

		if(v1->degree > 2)
		{
			(*subgradient)[v1-> index] += (*edges)[i]-> variable;
		}

		if(v1->degree > 2)
		{
			(*subgradient)[v2-> index] += (*edges)[i]-> variable;
		}
	}
}

double calc_step_size(int n_vertices, double current_lower_bound, int best_upper_bound, vector<int> *subgradient_vector)
{
	double epsilon, numerator, denominator, step_size;

	epsilon = 0.1;
	numerator = best_upper_bound - current_lower_bound;

	denominator = 0;
	for(int i = 0; i < n_vertices; i++)
	{
		denominator += pow((*subgradient_vector)[i], 2);
	}

	step_size = epsilon * (numerator/denominator);
	return step_size;
}

void update_best_lower_bound(int iteration, int *iteration_best_lower_bound,
	double *best_lower_bound, double current_lower_bound)
{
	if(*best_lower_bound < current_lower_bound)
		*iteration_best_lower_bound = iteration;
	*best_lower_bound = max(*best_lower_bound, current_lower_bound);	
}

void update_best_upper_bound(int n_edges, int iteration, int *iteration_best_upper_bound, 
	int *best_upper_bound, int current_upper_bound, vector<pair<int,int>> *best_upper_bound_solution, vector<Edge*> *edges)
{
	if(*best_upper_bound > current_upper_bound)
	{
		*iteration_best_upper_bound = iteration;
		int j = 0;
		for(int i = 0; i < n_edges; i++)
		{
			if((*edges)[i]-> variable)
			{
				(*best_upper_bound_solution)[j] = make_pair((*edges)[i]-> vertex_1 -> index, (*edges)[i]-> vertex_2 -> index);
				j++;
			}
		}
	}
	*best_upper_bound = min(*best_upper_bound, current_upper_bound);
}

void print_result(int n_vertices, double best_lower_bound, int iteration_best_lower_bound, int best_upper_bound, 
	int iteration_best_upper_bound, int iteration, vector<pair<int,int>> *best_upper_bound_solution)
{

	if(iteration_best_lower_bound != -1)
	{
		printf("%.6lf\n", best_lower_bound);
		printf("%d\n", iteration_best_lower_bound);
	}
	else
	{
		printf("-\n-\n");
	}

	if(iteration_best_upper_bound != -1)
	{
		printf("%d\n", best_upper_bound);
		printf("%d\n", iteration_best_upper_bound);
	}
	else
	{
		printf("-\n-\n");
	}

	printf("%d\n", iteration);
	for(int i = 0; i < n_vertices-1; i++)
	{
		printf("%d %d\n", (*best_upper_bound_solution)[i].first, (*best_upper_bound_solution)[i].second);
	}
}

bool check_opt_by_gap(double best_upper_bound, int best_lower_bound)
{
	return ceil(best_upper_bound) == best_lower_bound;
}

bool check_opt_by_sol(int n_vertices, int n_edges,  vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	vector<double>left_side(n_vertices, -2);

	for(int i = 0; i < n_edges; i++)
	{
		left_side[(*edges)[i]->vertex_1-> index] += (*edges)[i]-> variable;
		left_side[(*edges)[i]->vertex_2-> index] += (*edges)[i]-> variable;
	}

	for(int i = 0; i < n_vertices; i++)
	{
		if(left_side[i] > (*vertices)[i]-> degree * (*vertices)[i]-> variable ||
			(*vertices)[i]-> lambda > 0 && left_side[i] != (*vertices)[i]-> degree * (*vertices)[i]-> variable)
		{
			return false;
		}
	}

	return true;
}

void subgradient(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	// Vector of lambdas of langrarian relaxation

	vector<int> subgradient_vector(n_vertices);
	vector<pair<int,int>> best_upper_bound_solution(n_vertices-1);
	int iteration, current_upper_bound, best_upper_bound, iteration_best_upper_bound, iteration_best_lower_bound;
	double rl1_value, rl2_value, current_lower_bound, best_lower_bound, step_size;
	bool opt_by_gap_flag, opt_by_sol_flag;


	set_initial_lambdas(n_vertices, vertices);
	current_lower_bound = numeric_limits<int>::min();
	current_upper_bound = numeric_limits<int>::max();
	best_lower_bound = numeric_limits<int>::min();
	best_upper_bound = numeric_limits<int>::max();
	iteration_best_lower_bound = -1;
	iteration_best_upper_bound = -1;

	opt_by_gap_flag = opt_by_sol_flag = false;
	
	for(iteration = 0; iteration < 1000 && ceil(best_lower_bound) < best_upper_bound; iteration++)
	{
		// Executes Kruskal algorithm for getting a minimum spanning tree for RL1 problem
		rl1_value = kruskal(n_vertices, n_edges, vertices, edges);

		current_upper_bound = calc_sol_value(n_vertices, n_edges, vertices, edges);
		// Updates best upper bound
		update_best_upper_bound(n_edges, iteration, &iteration_best_upper_bound, &best_upper_bound,
			current_upper_bound, &best_upper_bound_solution, edges);

		// Executes inspection algorithm for RL2 problem
		rl2_value = inspection(n_vertices, vertices);
		
		// Calculates RL solution value
		current_lower_bound = get_lower_bound(n_vertices, vertices, rl1_value, rl2_value);
	
		opt_by_sol_flag = check_opt_by_sol(n_vertices, n_edges, vertices, edges);
		if(opt_by_sol_flag)
			break;

		// Updates best lower bound
		update_best_lower_bound(iteration, &iteration_best_lower_bound, &best_lower_bound, current_lower_bound);

		// Calculates subgradients vector
		fill(subgradient_vector.begin(), subgradient_vector.end(), 0);
		calc_subgradient(n_vertices, n_edges, vertices, edges, &subgradient_vector);

		// Executes heuristic
		current_upper_bound = heuristic(n_vertices, n_edges, vertices, edges);

		// Updates best upper bound
		update_best_upper_bound(n_edges, iteration, &iteration_best_upper_bound, &best_upper_bound,
			current_upper_bound, &best_upper_bound_solution, edges);

		opt_by_gap_flag = check_opt_by_gap(best_upper_bound, best_lower_bound);
		if(opt_by_gap_flag)
			break;

		// Calculates step size
		step_size = calc_step_size(n_vertices, current_lower_bound, best_upper_bound, &subgradient_vector);
		
		// Calculates new lambdas
		set_lambdas(n_vertices, step_size, vertices, &subgradient_vector);
	}

	print_result(n_vertices, best_lower_bound, iteration_best_lower_bound, best_upper_bound, 
	iteration_best_upper_bound, iteration, &best_upper_bound_solution);
}