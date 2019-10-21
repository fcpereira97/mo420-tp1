#include <bits/stdc++.h>
#include "agmr_rl2.h"
#include "mystruct.h"

using namespace std;

// Inspects variables by evaluating costs of RL2
double inspection(int n_vertices, vector<Vertex*> *vertices)
{
	// Value of rl2 problem
	
	double rl2_value = 0;

	for (int i = 0; i < n_vertices; i++)
	{
		// If cost is less than 0, then y_i is assigned with 1 value, else with 0 value
		double cost = 1.0 - (double)(*vertices)[i]-> degree * (*vertices)[i]-> lambda;
		if(cost <= 0)
		{
			(*vertices)[i]-> variable = true;
			rl2_value += cost;	
		}
	}
	
	return rl2_value;

}