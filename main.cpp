#include <iostream>
#include <vector>
#include <fstream>

#include "simpleGA.h"
#include "graph.h"

int main(char argc, char** argv) {
	srand(time(NULL));

	if (argc != 3) {
		std::cout << "Wrong number of arguments. Terminating\n";
		return -1;
	}
	
	std::ifstream fin(argv[1]);
	//std::ifstream fin("graph30.txt");
	int num_colors;
	int num_vertices;

	if (!fin.is_open()) {
		std::cout << "Could not open file. Terminating\n";
		return -1;
	}
	else {
		// std::cout << "Reading file \"" << argv[1] << "\"" << std::endl;

		fin >> num_colors >> num_vertices;

		graph read_graph{ num_vertices, num_colors };
		int src, dst;
		
		while (fin >> src >> dst) {
			read_graph.add_edge(src, dst);
		}

		std::cout << "Number of colors: " << num_colors << "\nNumber of vertices: " << num_vertices << "\nGraph structure\n";
		std::cout << read_graph;

		fin.close();

		std::vector<individual<graph>> initial_population;

		for (int i = 0; i < simpleGA_defaults::population_size; ++i) {
			initial_population.push_back(create_random_individual(read_graph, num_colors));
		}

		int dummy = 0;

		simpleGA<int, graph> algo(initial_population, dummy, fitness_fn, mating_fn, mutating_fn);
		unsigned generation = 0;

		while (algo.get_best().fitness > 0) {
			generation = algo.get_epoch();

			if (generation % 10) {
				std::cout << "Generation: " << generation << " - Best solution\n" << algo.get_best().genome << std::endl;
			}

			algo.next_generation();
		}

		std::cout << "Solution found: " << std::endl << algo.get_best().genome << ", after " << algo.get_epoch() << " epochs." << std::endl;
		
		try {
			std::ofstream fout(argv[2]);

			if (!fout.is_open()) {
				return -1;
			}
			else {
				std::cout << "Saving result in \"" << argv[2] << "\"" << "file" << std::endl;
				fout << algo << std::endl;
			}
		}
		catch (std::exception& e) {
			std::cout << "Could not open output file\n";

			return -1;
		}
	}

	return 0;
}