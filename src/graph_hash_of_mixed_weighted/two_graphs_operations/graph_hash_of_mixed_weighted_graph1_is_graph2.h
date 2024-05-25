#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

bool graph_hash_of_mixed_weighted_graph1_is_graph2(graph_hash_of_mixed_weighted& graph1, graph_hash_of_mixed_weighted& graph2) {

	double error_bound = 1e-8;

	for (auto it1 = graph1.hash_of_vectors.begin(); it1 != graph1.hash_of_vectors.end(); it1++) {
		int v = it1->first;
		double w_v = it1->second.vertex_weight;

		auto xx = graph2.hash_of_vectors.find(v);
		if (xx == graph2.hash_of_vectors.end()) { // v is not in graph2
			return false;
		}
		else {
			if (abs(w_v - xx->second.vertex_weight) > error_bound) { // weights of v are different
				return false;
			}
		}

		std::vector<std::pair<int, double>> v_adj_list_graph1 = graph1.adj_v_and_ec(v);
		std::vector<std::pair<int, double>> v_adj_list_graph2 = graph2.adj_v_and_ec(v);

		if (v_adj_list_graph1.size() != v_adj_list_graph2.size()) { // adj lists of v are different
			return false;
		}
		else {
			for (int i = v_adj_list_graph1.size() - 1; i >= 0; i--) {
				if (v_adj_list_graph1[i].first != v_adj_list_graph2[i].first || abs(v_adj_list_graph1[i].second - v_adj_list_graph2[i].second) > error_bound) { // adj lists of v are different
					return false;
				}
			}
		}
	}

	return true;
}

