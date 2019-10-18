#include <bits/stdc++.h>
#include "agmr_rl2.h"

using namespace std;

// Inspects variables by evaluating costs of RL2
double inspection(int n_vertices, vector<int> *vertices_degrees, vector<double> *vertices_lambdas, vector<bool> *vertices_variables)
{
	// Value of rl2 problem
	double rl2_value = 0;

	for (int i = 0; i < n_vertices; i++)
	{
		// If cost is less than 0, then y_i is assigned with 1 value, else with 0 value
		double cost = 1.0 - (double)((*vertices_degrees)[i] * (*vertices_lambdas)[i]);
		if(cost <= 0)
		{
			(*vertices_variables)[i] = true;
			rl2_value += cost;	
		}
	}

	return rl2_value;
}