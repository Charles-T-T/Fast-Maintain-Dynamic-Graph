#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>


void graph_hash_of_mixed_weighted_copy_graph_to_another_graph(graph_hash_of_mixed_weighted& target_graph, graph_hash_of_mixed_weighted& copied_graph) {

	/*time complexity: O(|V_copied_graph| +|E_copied_graph|);
	this is to merge copied_graph into target_graph*/

	for (auto it1 = copied_graph.hash_of_vectors.begin(); it1 != copied_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		double w_i = it1->second.vertex_weight;
		graph_hash_of_mixed_weighted_add_vertex(target_graph, i, w_i); // add vertex

		auto search = copied_graph.hash_of_hashs.find(i);
		if (search != copied_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double c_ij = it2->second;
					graph_hash_of_mixed_weighted_add_edge(target_graph, i, j, c_ij); // add edge
				}
			}
		}
		else {
			auto search2 = copied_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double c_ij = it2->second;
					graph_hash_of_mixed_weighted_add_edge(target_graph, i, j, c_ij); // add edge
				}
			}
		}
	}
}

