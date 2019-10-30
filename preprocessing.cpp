#include <bits/stdc++.h>
#include "preprocessing.h"

using namespace std;

// Initialize data structures of the graph
void initialize_graph(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	for(int i = 0; i < n_vertices; i++)
	{
		(*vertices)[i]-> level = -1;
		(*vertices)[i]-> marked = false;
		(*vertices)[i]-> incident_edges.clear();
	}

	for(int i = 0; i < n_edges; i++)
	{
		Vertex *v1, *v2;

		v1 = (*edges)[i]-> vertex_1;
		v2 = (*edges)[i]-> vertex_2;

		v1-> neighbors.push_back(v2);
		v2-> neighbors.push_back(v1);

		v1-> incident_edges.push_back((*edges)[i]);
		v2-> incident_edges.push_back((*edges)[i]);

		(*edges)[i]-> marked = false;
		(*edges)[i]-> arborescent = false;
	}
}

// Returns the unmarked vertex with maximum level
Vertex* get_vertex_max_level(int n_vertices, vector<Vertex*> *vertices)
{
	Vertex* vertex_max_level;
	int max_level;

	vertex_max_level = NULL;
	max_level = -1;

	for(int i = 0; i < n_vertices; i++)
	{
		if(!(*vertices)[i]-> marked && (*vertices)[i]-> level > max_level)
		{
			max_level = (*vertices)[i]-> level;
			vertex_max_level = (*vertices)[i];
		}
	}
	return vertex_max_level;
}

// Builds an arborescence in graph
void build_arborescence(int n_vertices, vector<Vertex*> *vertices)
{
	Vertex *current_vertex;

	do
	{	
		// Gets the unmkarked vertex with maximum level
		current_vertex = get_vertex_max_level(n_vertices, vertices);

		if(current_vertex != NULL)
		{
			Vertex *neighbor_level_negative = NULL;

			// Searchs for a neighbor with negative level
			for(list<Vertex*>::iterator neighbor = current_vertex-> neighbors.begin(); neighbor != current_vertex-> neighbors.end(); neighbor++)
			{
				if((*neighbor)-> level < 0)
					neighbor_level_negative = (*neighbor);
			}

			// If neighbor with negative level was not found
			if(neighbor_level_negative == NULL)
			{
				current_vertex-> marked = true; // Sets current marked 
			}
			else
			{
				// Else, inserts the edge between current and neighbor in arborescence
				for(list<Edge*>::iterator incident_edge = current_vertex-> incident_edges.begin();
				incident_edge != current_vertex-> incident_edges.end(); incident_edge++)
				{
					if((*incident_edge)-> vertex_1 -> index == neighbor_level_negative-> index ||
					(*incident_edge)-> vertex_2 -> index == neighbor_level_negative-> index)
					{
						neighbor_level_negative-> level = current_vertex-> level + 1;
						(*incident_edge)-> arborescent = true;
						break;
					}
				}
			}
		}
	} while(current_vertex != NULL);
}

// Clears the vector of incident edges of each vertex and add only the incident edges from arborescence
void update_vertices(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	for(int i = 0; i < n_vertices; i++)
	{
		(*vertices)[i]-> incident_edges.clear();
	}

	for(int i = 0; i < n_edges; i++)
	{
		if((*edges)[i]-> arborescent)
		{
			(*edges)[i]-> vertex_1-> incident_edges.push_back((*edges)[i]);
			(*edges)[i]-> vertex_2-> incident_edges.push_back((*edges)[i]);
		}
	}
}

// Detects all brigdes in graph
void identify_bridges(int n_edges, vector<Edge*> *edges)
{
	// For each edge in graph
	for(int i = 0; i < n_edges; i++)
	{	
		Vertex *v1, *v2;
		v1 = (*edges)[i]-> vertex_1;
		v2 = (*edges)[i]-> vertex_2;

		if(v2-> level > v1-> level)
			swap(v1, v2);

		// If edge is not in arborescence and extremes i and j have different levels
		if(!(*edges)[i]-> arborescent && v1 -> level > v2-> level)
		{
			(*edges)[i]-> marked = true; // Marks the edge

			// Marks all edges in path from i to j in arborescence
			while(v1-> index != v2-> index)
			{
				for(list<Edge*>::iterator arborescent_edge = v1-> incident_edges.begin();
				arborescent_edge != v1-> incident_edges.end(); arborescent_edge++)
				{
					Vertex *v3;
					if((*arborescent_edge)-> vertex_1 -> index != v1-> index)
						v3 = (*arborescent_edge)-> vertex_1;
					else
						v3 = (*arborescent_edge)-> vertex_2;

					if(v1-> level > v3-> level)
					{
						(*arborescent_edge)-> marked = true; 
						v1 = v3;
						break;
					}
				}
			}
		}
	}
}

// Preprocessing algorithm
void preprocessing(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	// General variables
	list<Edge*> arborescence;
	vector<int> incident_bridges(n_vertices, 0);

	// First steps
	initialize_graph(n_vertices, n_edges, vertices, edges);
	(*vertices)[0]-> level = 0;
	build_arborescence(n_vertices, vertices);
	update_vertices(n_vertices, n_edges, vertices, edges);
	identify_bridges(n_edges, edges);

	// For each edge in graph
	for(int i = 0; i < n_edges; i++)
	{
		// If is not marked
		if(!(*edges)[i]-> marked)
		{		
			// Fix it as a bridge
			(*edges)[i]-> fixed = true;
			(*edges)[i]-> variable = true;

			// Increment counter of incident brigdes of extremes
			Vertex *v1, *v2;
			v1 = (*edges)[i]-> vertex_1;
			v2 = (*edges)[i]-> vertex_2;

			incident_bridges[v1->index]++;
			incident_bridges[v2->index]++;
		}
	}

	// For each vertex, mark as a fixed if
	// Has two incident bridges and more than 3 neighbors OR
	// Has more than two incident bridges
	for(int i = 0; i < n_vertices; i++)
	{
		if((incident_bridges[i] == 2 && (*vertices)[i]-> degree > 2) || incident_bridges[i] > 2)
		{
			(*vertices)[i]-> fixed = true;
			(*vertices)[i]-> variable = true;
		}
	}
}