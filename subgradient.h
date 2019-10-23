#ifndef SUBGRADIENT_H
#define SUBGRADIENT_H

#include "mystruct.h"

using namespace std;

void subgradient(int n_vertices, int n_edges, vector<Vertex*> *vertices, vector<Edge*> *edges, time_t time_start, int time_limit, FILE* output_file);

#endif
