#ifndef MYSTRUCT_H
#define MYSTRUCT_H

using namespace std;

struct Vertex
{
	int index;
	int degree;
	double lambda;
	bool visited;
	list<Vertex*> neighbors;
};

typedef struct Vertex Vertex;

struct Edge
{
	int index;
	Vertex *vertex_1, *vertex_2;
	double weight;
};

typedef struct Edge Edge;


#endif