#ifndef MYSTRUCT_H
#define MYSTRUCT_H

using namespace std;

struct Vertex
{
	int index;
	int degree;
	double lambda;
	bool visited;
	bool variable;
	list<Vertex*> neighbors;
};

typedef struct Vertex Vertex;

struct Edge
{
	int index;
	double weight;
	bool variable;
	Vertex *vertex_1, *vertex_2;
};

typedef struct Edge Edge;


#endif