#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

/*this function generates a random graph with vertex and edge weights, and this graph
may not be connected.*/


#include <boost/random.hpp>
graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_generate_random_graph(long long int V, long long int E, double nw_min, double nw_max,
	double ec_min, double ec_max, int input_precision, boost::random::mt19937& boost_random_time_seed) { // must use long long int for large V E

	/*time complexity: O(|V||E|)*/

	double precision = std::pow(10, input_precision);
	boost::random::uniform_int_distribution<> dist_ec{ static_cast<int>(ec_min* precision), static_cast<int>(ec_max* precision) };
	boost::random::uniform_int_distribution<> dist_nw{ static_cast<int>(nw_min* precision), static_cast<int>(nw_max* precision) };

	/*time complexity: O(|V|)*/
	graph_hash_of_mixed_weighted random_graph; // generate vertices
	for (int i = 0; i < V; i++) {
		double new_weight = (double)dist_nw(boost_random_time_seed) / precision; // generate nw
		graph_hash_of_mixed_weighted_add_vertex(random_graph, i, new_weight);
	}

	/*add edges to random_graph*/
	long long int max_E = V * (V - 1) / 2; // must use long long int for large V
	if (E == max_E) { // complete graphs
		/*time complexity: O(|V|+|E|)*/
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < i; j++) {
				double new_cost = (double)dist_ec(boost_random_time_seed) / precision; // generate ec
				graph_hash_of_mixed_weighted_add_edge(random_graph, i, j, new_cost);
			}
		}
	}
	else if (E > max_E) {
		std::cout << "E: " << E << std::endl;
		std::cout << "V * (V - 1) / 2: " << max_E << std::endl;
		std::cout << "E > V * (V - 1) / 2 in graph_hash_of_mixed_weighted_generate_random_graph!" << '\n';
		exit(1);
	}
	else { // incomplete graphs

		/*time complexity: O(|V|)*/
		std::vector<int> not_full_vertices; // vertices without a full degree
		for (int i = 0; i < V; i++) {
			not_full_vertices.insert(not_full_vertices.end(), i);
		}


		/*time complexity: O(|V||E|)*/
		int edge_num = 0;
		while (edge_num < E) {
			boost::random::uniform_int_distribution<> dist_id
			{ static_cast<int>(0), static_cast<int>(not_full_vertices.size() - 1) };
			int RAND = dist_id(boost_random_time_seed); // generate int random number  0, not_full_vertices.size()-1
			if (graph_hash_of_mixed_weighted_degree(random_graph, not_full_vertices[RAND]) < V - 1) { // this is a vertex without a full degree


				/*time complexity: O(|V|)*/
				std::vector<int> unchecked(V);
				std::iota(std::begin(unchecked), std::end(unchecked), 0);
				bool added = false;
				while (added == false) {
					boost::random::uniform_int_distribution<> dist_id2
					{ static_cast<int>(0), static_cast<int>(unchecked.size() - 1) };
					int x = dist_id2(boost_random_time_seed);
					int j = unchecked[x];
					if (not_full_vertices[RAND] != j &&
						graph_hash_of_mixed_weighted_contain_edge(random_graph, not_full_vertices[RAND], j) == 0) {
						// This edge does not exist
						double new_cost = (double)dist_ec(boost_random_time_seed) / precision; // generate ec
						graph_hash_of_mixed_weighted_add_edge(random_graph, not_full_vertices[RAND], j, new_cost); // add a new edge
						edge_num++;
						added = true;
						break; // break after adding one edge
					}
					else {
						unchecked.erase(unchecked.begin() + x);
					}
				}




			}
			else { // this is a vertex with a full degree
				not_full_vertices.erase(not_full_vertices.begin() + RAND);
			}
		}


	}

	return random_graph;
}



