#ifndef graph_h
#define graph_h

#include "simpleGA.h"

struct graph {
	int vertices;
	int num_colors;
	std::vector<std::vector<int>> adjacency_matrix;
	std::vector<int> colors;

	graph(int v, int c) : vertices(v), num_colors(c), adjacency_matrix(v, std::vector<int>(v, 0)), colors(v, -1) {}

	void add_edge(int src, int dest) {
		adjacency_matrix[src][dest] = 1;
		adjacency_matrix[dest][src] = 1;
	}

	void set_color(int vertex, int color) {
		colors[vertex] = color;
	}

	friend std::ostream& operator<<(std::ostream& os, const graph& g) {
		for (int i = 0; i < g.vertices; ++i) {
			for (int j = 0; j < g.vertices; ++j) {
				os << g.adjacency_matrix[i][j] << " ";
			}
			os << std::endl;
		}

		os << "Colors of the Vertices:" << std::endl;
		for (int i = 0; i < g.vertices; ++i) {
			os << "Vertex " << i << ": " << g.colors[i] << std::endl;
		}

		return os;
	}
};

individual<graph> create_random_individual(const graph& pattern, int num_colors) {
	graph tmp(pattern.vertices, pattern.num_colors);

	std::copy(pattern.adjacency_matrix.begin(), pattern.adjacency_matrix.end(), tmp.adjacency_matrix.begin());

	for (auto& color : tmp.colors) {
		color = rand() % num_colors;
	}

	return tmp;
}

int fitness_fn(int& i, graph& g) {
	int f = 0;

	for (int i = 0; i < g.vertices; ++i) {
		for (int j = 0; j < g.vertices; ++j) {
			if (g.adjacency_matrix[i][j] == 1 && (g.colors[i] == g.colors[j])) {
				++f;
			}
		}
	}

	return f;
}

individual<graph> mating_fn(individual<graph>& g1, individual<graph>& g2) {
	graph child{ g1.genome.vertices, g1.genome.num_colors };
	std::copy(g1.genome.adjacency_matrix.begin(), g1.genome.adjacency_matrix.end(), child.adjacency_matrix.begin());

	for (int i = 0; i < g1.genome.vertices; ++i) {
		float p = get_rand_int(0, 100) / 100;

		if (p < 0.50) {
			child.colors[i] = g1.genome.colors[i];
		}
		else {
			child.colors[i] = g2.genome.colors[i];
		}
	}

	return child;
}

void mutating_fn(individual<graph>& g) {
	float p = get_rand_int(0, 100) / 100;

	if (p < 0.1) {
		g.genome.colors[get_rand_int(0, g.genome.vertices - 1)] = get_rand_int(0, g.genome.num_colors - 1);
	}
}

#endif