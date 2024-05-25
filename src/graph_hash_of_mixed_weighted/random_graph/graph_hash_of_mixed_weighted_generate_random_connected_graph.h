#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

#include <boost/random.hpp>
graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_generate_random_connected_graph(double V, double E, double nw_min, double nw_max,
	double ec_min, double ec_max, int input_precision) {

	/*time complexity: O(|V||E|)*/

	double precision = std::pow(10, input_precision);
	std::time_t now = std::time(0);
	boost::random::mt19937 gen{ static_cast<std::uint32_t>(now) };
	boost::random::uniform_int_distribution<> dist_ec{ static_cast<int>(ec_min*precision), static_cast<int>(ec_max*precision) };
	boost::random::uniform_int_distribution<> dist_nw{ static_cast<int>(nw_min*precision), static_cast<int>(nw_max*precision) };

	/*time complexity: O(|V|)*/
	graph_hash_of_mixed_weighted random_graph; // generate vertices
	for (int i = 0; i < V; i++) {
		double new_weight = (double)dist_nw(gen) / precision; // generate nw
		graph_hash_of_mixed_weighted_add_vertex(random_graph, i, new_weight);
	}

	/*add edges to random_graph*/
	if (E == V * (V - 1) / 2) { // complete graphs
		/*time complexity: O(|V|+|E|)*/
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < i; j++) {
				double new_cost = (double)dist_ec(gen) / precision; // generate ec
				graph_hash_of_mixed_weighted_add_edge(random_graph, i, j, new_cost);
			}
		}
	}
	else if (E > V* (V - 1) / 2) {
		std::cout << E << std::endl;
		std::cout << V * (V - 1) / 2 << std::endl;
		std::cout << "E > V * (V - 1) / 2 in graph_unordered_map_generate_random_connected_graph!" << '\n';
		exit(1);
	}
	else if (E < V - 1) {
		std::cout << "E < V - 1 in graph_unordered_map_generate_random_connected_graph!" << '\n';
		exit(1);
	}
	else { // incomplete graphs

		/*generate a random spanning tree;
		time complexity: O(|V|)*/
		std::vector<int> inside_V; // the included vertex
		inside_V.insert(inside_V.end(), 0);
		while (inside_V.size() < V) {

			boost::random::uniform_int_distribution<> dist_v{ static_cast<int>(0), static_cast<int>(inside_V.size() - 1) }; // generate random number from [0, inside_V.size()-1]
			int v1 = dist_v(gen);

			//int v1 = rand() % inside_V.size();  // generate random number from [0, inside_V.size()-1] // rand() only suits small range			

			int v2 = inside_V.size();
			double new_cost = (double)dist_ec(gen) / precision; // generate ec
			graph_hash_of_mixed_weighted_add_edge(random_graph, v1, v2, new_cost);
			inside_V.insert(inside_V.end(), v2);
		}

		/*time complexity: O(|V|)*/
		std::vector<int> not_full_vertices; // vertices without a full degree
		for (int i = 0; i < V; i++) {
			not_full_vertices.insert(not_full_vertices.end(), i);
		}


		/*time complexity: O(|V||E|)*/
		int edge_num = V - 1;
		while (edge_num < E) {
			boost::random::uniform_int_distribution<> dist_id
			{ static_cast<int>(0), static_cast<int>(not_full_vertices.size() - 1) };
			int RAND = dist_id(gen); // generate int random number  0, not_full_vertices.size()-1
			if (graph_hash_of_mixed_weighted_degree(random_graph, not_full_vertices[RAND]) < V - 1) { // this is a vertex without a full degree


				/*time complexity: O(|V|)*/
				std::vector<int> unchecked(V);
				std::iota(std::begin(unchecked), std::end(unchecked), 0);
				bool added = false;
				while (added == false) {
					boost::random::uniform_int_distribution<> dist_id2
					{ static_cast<int>(0), static_cast<int>(unchecked.size() - 1) };
					int x = dist_id2(gen);
					int j = unchecked[x];
					if (not_full_vertices[RAND] != j && graph_hash_of_mixed_weighted_contain_edge(random_graph, not_full_vertices[RAND], j) == 0) {
						// This edge does not exist
						double new_cost = (double)dist_ec(gen) / precision; // generate ec
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

