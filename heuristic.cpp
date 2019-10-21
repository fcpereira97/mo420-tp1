#include <bits/stdc++.h>
#include "heuristic.h"
#include "mystruct.h"

using namespace std;

void erase_incident_edges(int n_vertices, vector<Vertex*> *vertices)
{
	for(int i = 0; i < n_vertices; i++)
	{
		((*vertices)[i]-> incident_edges).clear();
	}
}

void erase_visitation(int n_vertices, vector<Vertex*> *vertices)
{
	for(int i = 0; i < n_vertices; i++)
	{
		(*vertices)[i]-> visited = false;
	}
}

void calc_ramifications(int n_edges, vector<Edge*> *edges)
{
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

void corect_vertices_variables(int n_vertices, vector<Vertex*> *vertices)
{
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> ramifications <= 2)
			(*vertices)[i]-> variable = false;
		else
			(*vertices)[i]-> variable = true;
	}
}

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
			//cout << v1->incident_edges.size() << endl;
			//cout << v1-> index << " " << next_v1 -> index << " " << v2-> index << endl;
			(*path).push_back(*incident_edge);

			if(next_v1 -> index != v2-> index)
			{
				find_path(next_v1, v2, path);
				if((*path).back()-> index == (*incident_edge)->index)
					(*path).pop_back();
			}
		}

		++incident_edge;

	} while(incident_edge != v1->incident_edges.end() && ((*path).empty() ||
		(*path).back()-> vertex_1 -> index != v2-> index &&
		(*path).back()-> vertex_2 -> index != v2 -> index));

	v1-> visited = false;
}

void change_edges(Edge *in_edge, list<Edge*> *path)
{
	Edge *out_edge = NULL;

	for (list<Edge*>::iterator cand_edge = (*path).begin(); cand_edge != (*path).end(); ++cand_edge)
	{
		if((*cand_edge)-> vertex_1 -> ramifications == 3 && (*cand_edge)-> vertex_2 -> ramifications == 3)
			out_edge = (*cand_edge);
		else if((*cand_edge)-> vertex_1 -> ramifications == 3 || (*cand_edge)-> vertex_2 -> ramifications == 3)
			out_edge = (*cand_edge);
	}

	if (out_edge != NULL)
	{
		out_edge-> variable = false;
		out_edge-> vertex_1 -> incident_edges.remove(out_edge);
		out_edge-> vertex_2 -> incident_edges.remove(out_edge);
		out_edge-> vertex_1 -> ramifications--;
		out_edge-> vertex_2 -> ramifications--;
		out_edge-> vertex_1 -> variable = out_edge-> vertex_1 -> ramifications > 2;
		out_edge-> vertex_2 -> variable = out_edge-> vertex_2 -> ramifications > 2;

		in_edge -> variable = true;
		in_edge -> vertex_1 -> incident_edges.push_back(in_edge);
		in_edge -> vertex_2 -> incident_edges.push_back(in_edge);
		in_edge-> vertex_1 -> ramifications++;
		in_edge-> vertex_2 -> ramifications++;
		in_edge-> vertex_1 -> variable = in_edge-> vertex_1 -> ramifications > 2;
		in_edge-> vertex_2 -> variable = in_edge-> vertex_2 -> ramifications > 2;
	}
}

int heuristic(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges)
{
	int upper_bound = 0;

	// Calculates ramifications on tree
	calc_ramifications(n_edges, edges);

	// Corrects vertices variables
	corect_vertices_variables(n_vertices, vertices);

	erase_incident_edges(n_vertices, vertices);

	erase_visitation(n_vertices, vertices);

	list<Edge*> available_edges;

	for(int i = 0; i < n_edges; i++)
	{
		Vertex *v1, *v2;
		v1 = (*edges)[i]-> vertex_1;
		v2 = (*edges)[i]-> vertex_2;

		if((*edges)[i]-> variable)
		{
			//cout << (*edges)[i] -> vertex_1 -> index << " " << (*edges)[i] -> vertex_2 -> index << endl;
			v1-> incident_edges.push_back((*edges)[i]);
			v2-> incident_edges.push_back((*edges)[i]);
		}
		else if(v1-> ramifications != 2 && v2->ramifications != 2)
		{
			available_edges.push_back((*edges)[i]); 
		}
	}
	
	for(list<Edge*>::iterator available_edge = available_edges.begin(); available_edge != available_edges.end(); ++available_edge)
	{
		list<Edge*> path;
		Vertex *v1, *v2;

		v1 = (*available_edge)-> vertex_1;
		v2 = (*available_edge)-> vertex_2;
		//cout << "FIND " << v1-> index << " " << v2-> index << endl;
		find_path(v1, v2, &path);
		change_edges( *available_edge, &path);
	}
	
	for(int i = 0; i < n_vertices; i++)
	{
		if((*vertices)[i]-> variable)
			upper_bound++;
	}

	return upper_bound;
}