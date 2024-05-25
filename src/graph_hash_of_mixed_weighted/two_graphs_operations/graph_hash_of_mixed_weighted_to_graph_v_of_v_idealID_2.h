#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <graph_v_of_v_idealID/graph_v_of_v_idealID.h>

graph_v_of_v_idealID graph_hash_of_mixed_weighted_to_graph_v_of_v_idealID_2(graph_hash_of_mixed_weighted& input_graph, int max_N) {

	graph_v_of_v_idealID output_graph(max_N);

	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double ec = it2->second;
					graph_v_of_v_idealID_add_edge(output_graph, i, j, ec);
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double ec = it2->second;
					graph_v_of_v_idealID_add_edge(output_graph, i, j, ec);
				}
			}
		}
	}

	return output_graph;
}