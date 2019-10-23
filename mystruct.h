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
	int level;
	double lambda;
	bool visited;
	bool marked;
	bool variable;
	bool fixed;
	list<Edge*> incident_edges;
	list<Vertex*> neighbors;
};

struct Edge
{
	int index;
	double weight;
	bool variable;
	bool marked;
	bool arborescent;
	bool fixed;
	Vertex *vertex_1, *vertex_2;
};

#endif