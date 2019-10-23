#include <bits/stdc++.h>
#include "inspection.h"
#include "mystruct.h"

using namespace std;

void erase_variables(int n_vertices, vector<Vertex*> *vertices)
{
	for(int i = 0; i < n_vertices; i++)
		if(!(*vertices)[i]-> fixed)
			(*vertices)[i]-> variable = false;
}

// Inspects variables by evaluating costs of RL2
double inspection(int n_vertices, vector<Vertex*> *vertices)
{
	// Value of rl2 problem
	double rl2_value = 0;

	erase_variables(n_vertices, vertices);

	for (int i = 0; i < n_vertices; i++)
	{
		// If cost is less than 0, then y_i is assigned with 1 value, else with 0 value
		double cost = 1.0 - (double)(*vertices)[i]-> degree * (*vertices)[i]-> lambda;
		if((*vertices)[i]-> fixed)
		{
			rl2_value += cost;
		}
		else if(cost <= 0)
		{
			(*vertices)[i]-> variable = true;
			rl2_value += cost;	
		} 
	}
	
	return rl2_value;

}