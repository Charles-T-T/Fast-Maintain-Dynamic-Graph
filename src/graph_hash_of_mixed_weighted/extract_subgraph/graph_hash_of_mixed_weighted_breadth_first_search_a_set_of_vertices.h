#pragma once
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

/* this function uses breadth_first_search to find all vertices that connect to root, 
including root; time complexity: O(V + E) */

std::unordered_set<int> graph_hash_of_mixed_weighted_breadth_first_search_a_set_of_vertices(graph_hash_of_mixed_weighted& input_graph, int root) {

	std::unordered_set<int> root_component; // v is connected to root; including root

	std::unordered_map<int, bool> unprocessed;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		if (it->first == root) {
			unprocessed[it->first] = false; // mark root as discovered
		}
		else {
			unprocessed[it->first] = true; // mark non-root as un-discovered
		}
	}
	std::queue<int> Q; // Queue is a data structure designed to operate in FIFO (First in First out) context.
	Q.push(root);
	while (Q.size() > 0) {
		int v = Q.front();
		root_component.insert(v); // v is connected to root; including root
		Q.pop(); //Removing that vertex from queue,whose neighbour will be visited now
		/*processing all the neighbours of v*/
		auto search = input_graph.hash_of_hashs.find(v);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int vertex = it2->first;
				auto pointer_vertex = unprocessed.find(vertex);
				if (pointer_vertex->second == true) { // vertex has not been discovered
					Q.push(vertex);
					pointer_vertex->second = false;  // vertex has been discovered
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int vertex = it2->first;
				auto pointer_vertex = unprocessed.find(vertex);
				if (pointer_vertex->second == true) { // vertex has not been discovered
					Q.push(vertex);
					pointer_vertex->second = false;  // vertex has been discovered
				}
			}
		}

	}

	return root_component;

}