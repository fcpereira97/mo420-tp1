#include <bits/stdc++.h>
#include "agmr_rl1.h"

using namespace std;

// Compare function used to sort edges by weights
bool compare(const pair<int, int>&i, const pair<int, int>&j)
{
    return i.second < j.second;
}

// Calculates edges' weights based on lambda values
void calc_weights(int n_edges, vector<pair<int,int>> *edges, vector<double> *vertices_lambdas, vector<pair<int,double>> *edges_weights)
{
	for(int i = 0; i < n_edges; i++)
	{
		int v1, v2;
		double weight;

		// Extremities of edge i
		v1 = (*edges)[i].first;
		v2 = (*edges)[i].second;

		// Calculates weight
		weight = (*vertices_lambdas)[v1] + (*vertices_lambdas)[v2];
		// Creates edge with weight
		(*edges_weights)[i] = make_pair(i, weight);
	}
}

// Initializes disjoint setes of Union Find, creating a set for each edge
void initialize_union_roots(int n_vertices, vector<pair<int,int>> *union_roots)
{
	for(int i = 0; i < n_vertices; i++)
	{
		(*union_roots)[i] = make_pair(i, i);
	}
}

// Finds the root of a element using path compression technique
int find_root(vector<pair<int,int>> *union_roots, int vertex)
{
	while((*union_roots)[vertex].first != vertex)
	{
		(*union_roots)[vertex].first = (*union_roots)[(*union_roots)[vertex].first].first;
		vertex = (*union_roots)[vertex].first;
	}
	return vertex;
}

// Checks if two edges belong to the same set
bool check_sets(vector<pair<int,int>> *union_roots, int v1, int v2)
{
	return find_root(union_roots, (*union_roots)[v1].first) == find_root(union_roots, (*union_roots)[v2].first);
}

// Makes the union of two disjoint sets
void make_union(vector<pair<int,int>> *union_roots, int v1, int v2)
{
	int root_v1 = find_root(union_roots, v1);
	int root_v2 = find_root(union_roots, v2);

	// Makes the union based on the size of sets aiming to keep the resultant tree quite balanced
	if((*union_roots)[root_v1].second < (*union_roots)[root_v2].second)
	{
		(*union_roots)[root_v1].first = root_v2;
		(*union_roots)[root_v2].second += (*union_roots)[root_v1].second;
	}
	else
	{
		(*union_roots)[root_v2].first = root_v1;
		(*union_roots)[root_v1].second += (*union_roots)[root_v2].second;
	}
}


// Main function of Kruskal's algorithm
double kruskal(int n_vertices, int n_edges, vector<int> *vertices_degrees, vector<pair<int,int>> *edges, vector<double> *vertices_lambdas, vector<bool> *edges_variables)
{
	// Array of edges
	//First atribute corresponds to edge's index, the second one corresponds to edge's weight
	vector<pair<int,double>> edges_weights(n_edges);

	// Array of disjoint sets used on Union Find data structure
	// For a edge i:
	// First atribute corresponds to the root of i and the second one corresponds to the size of its set, if i is a root
	vector<pair<int, int>> union_roots(n_vertices);

	// Array of edges' boolean variables
	int tree_size, next_edge;
	double tree_weight;

	// Calculate sedges' weights
	calc_weights(n_edges, edges, vertices_lambdas, &edges_weights);

	// Sorts edges ascending by weights
	sort(edges_weights.begin(), edges_weights.end(), compare);

	// Initializes Union Find
	initialize_union_roots(n_vertices, &union_roots);

	// Initializes Kruskal' Algorithm variables
	tree_size = 0;
	tree_weight = 0;
	next_edge = 0;
	while(tree_size < n_vertices - 1 && next_edge < n_edges) 
	{
		int edge, v1, v2;
		edge = edges_weights[next_edge].first; // Selects the edge with less weight
		v1 = (*edges)[edge].first;
		v2 = (*edges)[edge].second;

		// Checks whether edge extremities belong to distinct sets 
		// If so, then select the edge will result in a cycle!
		// If not, the edge can be added to the tree
		if((*vertices_degrees)[v1] > 2 && (*vertices_degrees)[v2] > 2 && !check_sets(&union_roots, v1, v2)) 
		{
			make_union(&union_roots, v1, v2); // Makes union of disjoint sets conected by the edge
			// Updates variables
			(*edges_variables)[edge] = true;
			tree_size++;
			tree_weight += edges_weights[next_edge].second;
		}
		next_edge++;
	}

	/*
	// Print tree
	for (int i = 0; i < tree_size; ++i)
	{
		if(edges_variables[i])
			cout << (*edges)[i].first << " " << (*edges)[i].second << endl;
	}
	cout << tree_weight << endl;
	*/

	return tree_weight;
}