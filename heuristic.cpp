#include <bits/stdc++.h>
#include "heuristic.h"
#include "mystruct.h"

using namespace std;


// Erases list of incident edges of all vertices
void erase_incident_edges(int n_vertices, vector<Vertex*> *vertices)
{
	for(int i = 0; i < n_vertices; i++)
	{
		((*vertices)[i]-> incident_edges).clear();
	}
}

// Erases the visitation flag of all vertices
void erase_visitation(int n_vertices, vector<Vertex*> *vertices)
{
	for(int i = 0; i < n_vertices; i++)
	{
		(*vertices)[i]-> visited = false;
	}
}

// Calculate the number of ramifications of a vertex, based on the incidence edges list
void calc_ramifications(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
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
}

// Corrects the value of vertices variables 
void correct_vertices_variables(int n_vertices, vector<Vertex*> *vertices)
{
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> ramifications <= 2)
			(*vertices)[i]-> variable = false;
		else
			(*vertices)[i]-> variable = true;
	}
}

// Find the path between vertices, based on their incidence edges lists
void find_path(Vertex *v1, Vertex *v2, list<Edge*> *path)
{
	list<Edge*>::iterator incident_edge = v1->incident_edges.begin();
	Vertex *next_v1;
	v1-> visited = true;

	do{
		if((*incident_edge)-> vertex_1 -> index != v1 -> index)
			next_v1 = (*incident_edge)-> vertex_1;
		else
			next_v1 = (*incident_edge)-> vertex_2;
		if(!next_v1-> visited)
		{
			(*path).push_back(*incident_edge);

			if(next_v1 -> index != v2-> index)
			{
				find_path(next_v1, v2, path);
				if((*path).back()-> index == (*incident_edge)->index)
					(*path).pop_back();
			}
		}

		++incident_edge;

	} while(incident_edge != v1->incident_edges.end() &&
		((*path).empty() ||
		((*path).back()-> vertex_1 -> index != v2-> index &&
		 (*path).back()-> vertex_2 -> index != v2-> index)));

	v1-> visited = false;
}

// Exchange a edge of the solution for another outer the solution
void exchange_edges(Edge *in_edge, Edge *out_edge)
{
	out_edge-> variable = false;
	out_edge-> vertex_1-> incident_edges.remove(out_edge);
	out_edge-> vertex_2-> incident_edges.remove(out_edge);
	out_edge-> vertex_1-> ramifications--;
	out_edge-> vertex_2-> ramifications--;
	out_edge-> vertex_1-> variable = out_edge-> vertex_1-> ramifications > 2;
	out_edge-> vertex_2-> variable = out_edge-> vertex_2-> ramifications > 2;

	in_edge-> variable = true;
	in_edge-> vertex_1-> incident_edges.push_back(in_edge);
	in_edge-> vertex_2-> incident_edges.push_back(in_edge);
	in_edge-> vertex_1-> ramifications++;
	in_edge-> vertex_2-> ramifications++;
	in_edge-> vertex_1-> variable = in_edge-> vertex_1-> ramifications > 2;
	in_edge-> vertex_2-> variable = in_edge-> vertex_2-> ramifications > 2;
}

// IMP1 algorithm of Martín (2015)
// Returns if occurred an improvement on solution value
bool improvement_1(list<Edge*> *available_edges)
{	
	bool improved = false;
	list<Edge*> available_edges_aux;

	// For each edge out of solution
	for(list<Edge*>::iterator available_edge = (*available_edges).begin(); available_edge != (*available_edges).end(); ++available_edge)
	{
		Vertex *v1, *v2;
		Edge* in_edge = *available_edge;
		v1 = in_edge-> vertex_1;
		v2 = in_edge-> vertex_2;

		// Checks if the edge's extremities v1 v2 have ramifications different of 2
		if(v1-> ramifications != 2 && v2->ramifications != 2)
		{
			list<Edge*> path;

			// Gets the path between v1 and v2 in the solution
			find_path(v1, v2, &path);

			// Checks if there's an edge inside the path with an extreme with ramification equals to 3 
			Edge *out_edge = NULL;
			for (list<Edge*>::iterator candidate_out_edge = path.begin(); candidate_out_edge != path.end(); ++candidate_out_edge)
			{
				if(((*candidate_out_edge)-> vertex_1 -> ramifications == 3 &&
				   (*candidate_out_edge)-> vertex_1 -> index != v1-> index &&
				   (*candidate_out_edge)-> vertex_1 -> index != v2-> index)
				   ||
				   ((*candidate_out_edge)-> vertex_2 -> ramifications == 3 &&
				   (*candidate_out_edge)-> vertex_2 -> index != v1-> index &&
				   (*candidate_out_edge)-> vertex_2 -> index != v2-> index))
				{
					out_edge = (*candidate_out_edge);
					break;
				}
			}

			// If the second edge exists
			if (out_edge != NULL)
			{
				improved = true;
				// Exchanges them
				exchange_edges(in_edge, out_edge);
				available_edges_aux.push_back(out_edge);
			}
			else
			{
				available_edges_aux.push_back(in_edge);
			}
		}
	}

	// Updates availables edges (the edges outter from solution)
	(*available_edges).clear();
	(*available_edges).assign(available_edges_aux.begin(), available_edges_aux.end());

	return improved;
}

// IMP2 algorithm of Martín (2015)
// Returns if occurred an improvement on solution value
bool improvement_2(list<Edge*> *available_edges)
{	
	bool improved = false;
	list<Edge*> available_edges_aux;

	// For each edge out of solution
	for(list<Edge*>::iterator available_edge = (*available_edges).begin(); available_edge != (*available_edges).end(); ++available_edge)
	{
		Vertex *v1, *v2;
		Edge* in_edge = *available_edge;
		v1 = in_edge-> vertex_1;
		v2 = in_edge-> vertex_2;

		// Checks if the edge's any of extremities v1 v2 has ramification different of 2
		if(v1-> ramifications != 2 || v2-> ramifications != 2)
		{
			list<Edge*> path;

			// Gets the path between v1 and v2 from the solution
			find_path(v1, v2, &path);

			Edge *out_edge = NULL;

			// If v1 or v2 has an neighbor v3 with ramification equals to 3, then selects the edge
			// from the cycle that incides over v3
			if(v1-> ramifications != 2)
			{
				Vertex *v3;
				if(path.back()-> vertex_1 -> index != v2-> index)
					v3 = path.back()-> vertex_1;
				else
					v3 = path.back()-> vertex_2;

				if(v3-> ramifications == 3)
				{
					out_edge = path.back();
				}
			}

			if(v2-> ramifications != 2)
			{
				Vertex *v3;
				if(path.front()-> vertex_1 -> index != v1-> index)
					v3 = path.front()-> vertex_1;
				else
					v3 = path.front()-> vertex_2;

				if(v3-> ramifications == 3)
				{
					out_edge = path.front();
				}
			}

			// If the second edge was found
			if (out_edge != NULL)
			{
				improved = true;
				// Exchanges them
				exchange_edges(in_edge, out_edge);
				available_edges_aux.push_back(out_edge);
			}
			else
			{
				available_edges_aux.push_back(in_edge);
			}
		}
	}

	// Updates the set of available edges (the ones outer from solution)
	(*available_edges).clear();
	(*available_edges).assign(available_edges_aux.begin(), available_edges_aux.end());

	return improved;
}

// Heuristic algorithm based on heuristics of Martín (2015)
int heuristic(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	// General variables
	int upper_bound = 0;
	list<Edge*> available_edges;

	// First steps
	calc_ramifications(n_vertices, n_edges, vertices, edges);
	correct_vertices_variables(n_vertices, vertices);
	erase_incident_edges(n_vertices, vertices);
	erase_visitation(n_vertices, vertices);

	// Sets the incident edges lists of the solution
	// and get the set of available edges
	for(int i = 0; i < n_edges; i++)
	{
		Vertex *v1, *v2;
		v1 = (*edges)[i]-> vertex_1;
		v2 = (*edges)[i]-> vertex_2;

		if((*edges)[i]-> variable)
		{
			v1-> incident_edges.push_back((*edges)[i]);
			v2-> incident_edges.push_back((*edges)[i]);
		}
		else
		{
			available_edges.push_back((*edges)[i]); 
		}
	}

	// Excecutes alternatively IMP1 and IMP2 until there's no improvement
	while(improvement_1(&available_edges) || improvement_2(&available_edges));

	// Calculates the solution value
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> variable)
			upper_bound++;
	}

	return upper_bound;
}