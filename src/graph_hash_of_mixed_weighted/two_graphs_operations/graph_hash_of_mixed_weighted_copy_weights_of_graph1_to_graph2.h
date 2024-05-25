#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

void graph_hash_of_mixed_weighted_copy_weights_of_graph1_to_graph2
(graph_hash_of_mixed_weighted& graph1, graph_hash_of_mixed_weighted& graph2) {

	/*we assume that the adjacency list of graph2 is in graph1*/

	/*time complexity: O(|V_graph2| +|E_graph2|)*/

	for (auto it1 = graph2.hash_of_vectors.begin(); it1 != graph2.hash_of_vectors.end(); it1++) {
		int i = it1->first;

		auto it3 = graph1.hash_of_vectors.find(i);
		it1->second.vertex_weight = it3->second.vertex_weight; // copy nw

		auto search = graph2.hash_of_hashs.find(i);
		if (search != graph2.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				it2->second = graph_hash_of_mixed_weighted_edge_weight(graph1, i, j); // copy ec
			}
		}
		else {
			auto search2 = graph2.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				it2->second = graph_hash_of_mixed_weighted_edge_weight(graph1, i, j); // copy ec
			}
		}
	}
}
