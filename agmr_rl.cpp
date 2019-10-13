#include <bits/stdc++.h>

using namespace std;

// Load number of vertices and edges of the graph
void load_graph_size(FILE* input_file, int *n_vertices, int *n_edges)
{
	fscanf(input_file, "%d %d", n_vertices, n_edges);
}

void load_edges(FILE* input_file, int n_edges, vector<pair<int,int>> * edges, vector<int> *vertices_degrees)
{
	int v1, v2;

	for(int i = 0; i < n_edges; i++) 
	{
		fscanf(input_file, "%d %d", &v1, &v2);
		(*edges)[i] = make_pair(v1 - 1, v2 - 1);
		(*vertices_degrees)[v1-1]++;
		(*vertices_degrees)[v2-1]++;
	}
}

int main (int argc, char *argv[])
{
	string input_path, output_path; 
	FILE *input_file;
	FILE *output_file;

	input_path = argv[1];
	output_path = argv[2];
	input_file = fopen(input_path.c_str(), "r");
	output_file = fopen(output_path.c_str(), "a");

	int n_vertices, n_edges;
	load_graph_size(input_file, &n_vertices, &n_edges);
	vector<pair<int,int>> edges(n_edges);
	vector<int> vertices_degrees (n_vertices, 0);

	load_edges(input_file, n_edges, &edges, &vertices_degrees);

	fclose(input_file);
	fclose(output_file);

	return 0;
}