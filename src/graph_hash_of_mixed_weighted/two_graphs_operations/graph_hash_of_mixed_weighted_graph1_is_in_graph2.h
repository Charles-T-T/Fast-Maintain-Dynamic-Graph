#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

bool graph_hash_of_mixed_weighted_graph1_is_in_graph2(graph_hash_of_mixed_weighted& graph1, graph_hash_of_mixed_weighted& graph2) {

	/*this function returns true is every vertex and edge in graph1 is also in graph2; this function does not compare weight*/

	for (auto it1 = graph1.hash_of_vectors.begin(); it1 != graph1.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		if (graph_hash_of_mixed_weighted_contain_vertex(graph2, i) == false) { // this vertex is not in graph2
			return false;
		}
		auto search = graph1.hash_of_hashs.find(i);
		if (search != graph1.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					if (graph_hash_of_mixed_weighted_contain_edge(graph2, i, j) == false) { // this edge is not in graph2
						return false;
					}
				}
			}
		}
		else {
			auto search2 = graph1.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					if (graph_hash_of_mixed_weighted_contain_edge(graph2, i, j) == false) { // this edge is not in graph2
						return false;
					}
				}
			}
		}
	}

	return true;
}
