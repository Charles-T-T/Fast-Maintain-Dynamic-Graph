#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>


/* T can be vector<int>, or list<int>, or unordered_set<int>*/
template <typename T>
graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_extract_subgraph(graph_hash_of_mixed_weighted& input_graph, T& list_of_v) {

	/*extract a smaller_graph, which contains all the vertices in list_of_v,
	and all the edges between vertices in list_of_v;
	time complexity O(|V_list|+|adj_v of V_list in input_graph|)*/

	/*time complexity O(|V_list|)*/
	std::unordered_set<int> list_unordered_set;
	for (auto i = list_of_v.begin(); i != list_of_v.end(); i++) {
		list_unordered_set.insert(*i);
	}

	/*time complexity O(|V_list|+|adj_v of V_list in input_graph|)*/
	graph_hash_of_mixed_weighted smaller_graph;
	for (auto i = list_of_v.begin(); i != list_of_v.end(); i++) {
		int v1 = *i;
		double nw = input_graph.hash_of_vectors[v1].vertex_weight;
		graph_hash_of_mixed_weighted_add_vertex(smaller_graph, v1, nw); // add vertex

		auto search = input_graph.hash_of_hashs.find(v1);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int v2 = it2->first;
				double ec = it2->second;
				if ((list_unordered_set.count(v2) > 0) && (v2 > v1)) { // v2 is in the list and only add edge once
					graph_hash_of_mixed_weighted_add_edge(smaller_graph, v1, v2, ec); // add edge
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(v1);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int v2 = it2->first;
				double ec = it2->second;
				if ((list_unordered_set.count(v2) > 0) && (v2 > v1)) { // v2 is in the list and only add edge once
					graph_hash_of_mixed_weighted_add_edge(smaller_graph, v1, v2, ec); // add edge
				}
			}
		}
	}

	return smaller_graph;

}
