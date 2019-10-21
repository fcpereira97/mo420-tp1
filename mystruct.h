#ifndef MYSTRUCT_H
#define MYSTRUCT_H

using namespace std;

typedef struct Edge Edge;
typedef struct Vertex Vertex;

struct Vertex
{
	int index;
	int degree;
	int ramifications;
	double lambda;
	bool visited;
	bool variable;
	list<Edge*> incident_edges;
};

struct Edge
{
	int index;
	double weight;
	bool variable;
	Vertex *vertex_1, *vertex_2;
};




#endif