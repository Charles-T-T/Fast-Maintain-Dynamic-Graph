#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_merge_graph_hash_of_mixed_weighted(graph_hash_of_mixed_weighted& g1, graph_hash_of_mixed_weighted& g2) {

	graph_hash_of_mixed_weighted merged_g = graph_hash_of_mixed_weighted_copy_graph(g1);

	for (auto it = g2.hash_of_vectors.begin(); it != g2.hash_of_vectors.end(); it++) {
		int v = it->first;
		double v_nw = it->second.vertex_weight;
		graph_hash_of_mixed_weighted_add_vertex(merged_g, v, v_nw); // copy vertex

		auto search = g2.hash_of_hashs.find(v);
		if (search != g2.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int adj_v = it2->first;
				double ec = it2->second;
				if (v < adj_v) {
					graph_hash_of_mixed_weighted_add_edge(merged_g, v, adj_v, ec); // copy edge
				}
			}
		}
		else {
			auto search2 = g2.hash_of_vectors.find(v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int adj_v = it2->first;
				double ec = it2->second;
				if (v < adj_v) {
					graph_hash_of_mixed_weighted_add_edge(merged_g, v, adj_v, ec); // copy edge
				}
			}
		}
	}

	return merged_g;

}




void graph_hash_of_mixed_weighted_merge_g2_into_g1(graph_hash_of_mixed_weighted& g1, graph_hash_of_mixed_weighted& g2) {

	for (auto it = g2.hash_of_vectors.begin(); it != g2.hash_of_vectors.end(); it++) {
		int v = it->first;
		g1.hash_of_vectors[v].vertex_weight = it->second.vertex_weight; // copy vertex

		auto search = g2.hash_of_hashs.find(v);
		if (search != g2.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int adj_v = it2->first;
				if (v < adj_v) {
					graph_hash_of_mixed_weighted_add_edge(g1, v, adj_v, it2->second); // copy edge
				}
			}
		}
		else {
			auto search2 = g2.hash_of_vectors.find(v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int adj_v = it2->first;
				if (v < adj_v) {
					graph_hash_of_mixed_weighted_add_edge(g1, v, adj_v, it2->second); // copy edge
				}
			}
		}
	}

}
