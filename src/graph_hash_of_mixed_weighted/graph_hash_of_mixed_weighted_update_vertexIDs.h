#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

template<typename T> // T can be std::unordered_map<int, int>, or vector<int>
graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_update_vertexIDs(graph_hash_of_mixed_weighted& input_graph, T& vertex_IDs_input_to_out_put) {

	graph_hash_of_mixed_weighted output_graph;

	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		graph_hash_of_mixed_weighted_add_vertex(output_graph, vertex_IDs_input_to_out_put[i], it1->second.vertex_weight);

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double ec = it2->second;
					graph_hash_of_mixed_weighted_add_edge(output_graph, vertex_IDs_input_to_out_put[i], vertex_IDs_input_to_out_put[j], ec);
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double ec = it2->second;
					graph_hash_of_mixed_weighted_add_edge(output_graph, vertex_IDs_input_to_out_put[i], vertex_IDs_input_to_out_put[j], ec);
				}
			}
		}
	}

	return output_graph;

}

