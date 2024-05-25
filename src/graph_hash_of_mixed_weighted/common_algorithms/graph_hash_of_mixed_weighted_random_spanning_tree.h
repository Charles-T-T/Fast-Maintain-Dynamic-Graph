#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <graph_hash_of_mixed_weighted/common_algorithms/graph_hash_of_mixed_weighted_minimum_spanning_tree.h>

graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_random_spanning_tree(graph_hash_of_mixed_weighted& input_graph) {

	/*input_graph may be disconnected; SmartSpanningTrees are forests; time complexity: O(|E|+|V|log|V|);
	
	returned tree has same weights with input graph*/

	/*a new graph with random edge costs; time complexity: O(E)*/
	double ec_min = 1, ec_max = 10;
	graph_hash_of_mixed_weighted new_graph = graph_hash_of_mixed_weighted_copy_graph(input_graph);
	for (auto it = new_graph.hash_of_vectors.begin(); it != new_graph.hash_of_vectors.end(); it++) {
		int v = it->first;
		auto search = new_graph.hash_of_hashs.find(v);
		if (search != new_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int vertex = it2->first;
				if (v < vertex) {
					int new_cost = ec_min + (rand() % (int)(ec_max - ec_min + 1)); // generate int random number 
					graph_hash_of_mixed_weighted_add_edge(new_graph, v, vertex, new_cost);// change edge weights
				}
			}
		}
		else {
			auto search2 = new_graph.hash_of_vectors.find(v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int vertex = it2->first;
				if (v < vertex) {
					int new_cost = ec_min + (rand() % (int)(ec_max - ec_min + 1)); // generate int random number 
					graph_hash_of_mixed_weighted_add_edge(new_graph, v, vertex, new_cost);// change edge weights
				}
			}
		}
	}

	/*add dummy vertices and edges for disconnected input_graph; time complexity: O(V)*/
	for (auto it = new_graph.hash_of_vectors.begin(); it != new_graph.hash_of_vectors.end(); it++) {
		int v = it->first;
		graph_hash_of_mixed_weighted_add_edge(new_graph, v, INT_MAX, INT_MAX);
	}

	/*find MST; time complexity: O(|E|+|V|log|V|)*/
	std::unordered_map<int, int> predessors = graph_hash_of_mixed_weighted_minimum_spanning_tree(new_graph);

	/*build SmartSpanningTrees*/
	graph_hash_of_mixed_weighted RandomSpanningTrees;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		int v = it->first;
		double nw_v = it->second.vertex_weight;
		graph_hash_of_mixed_weighted_add_vertex(RandomSpanningTrees, v, nw_v); // add all vertices
	}
	for (auto it = predessors.begin(); it != predessors.end(); ++it) {
		if (it->first != it->second && it->first != INT_MAX && it->second != INT_MAX) { // non-dummy edges in SmartSpanningTree
			double ec = graph_hash_of_mixed_weighted_edge_weight(input_graph, it->first, it->second);
			graph_hash_of_mixed_weighted_add_edge(RandomSpanningTrees, it->first, it->second, ec);
		}
	}

	return RandomSpanningTrees;

}