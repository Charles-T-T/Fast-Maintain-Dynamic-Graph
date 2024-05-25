#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <queue>
#include <unordered_map>

std::vector<std::pair<int, int>> 
graph_hash_of_mixed_weighted_breadth_first_search_a_tree_of_edges(graph_hash_of_mixed_weighted& input_graph, int& root) {

	/*the Pseudocode is in https://www.hackerearth.com/zh/practice/algorithms/graphs/breadth-first-search/tutorial/ */
	/*we assume that input_graph is connected,
	otherwise the components that are disconnected with the root will not be traversed*/

	std::vector<std::pair<int, int>> discovered_edges;

	std::unordered_map<int, bool> discovered;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		int v = it->first;
		if (v == root) {
			discovered[v] = true; // mark root as visited
		}
		else {
			discovered[v] = false; // mark non-root as un-visited
		}
	}

	std::queue<int> Q; // Queue is a data structure designed to operate in FIFO (First in First out) context.
	Q.push(root);

	while (Q.size() > 0) {

		int v = Q.front();
		Q.pop(); //Removing that vertex from queue,whose neighbour will be visited now

		/*processing all the neighbours of v*/
		auto search = input_graph.hash_of_hashs.find(v);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int vertex = it2->first;
				if (discovered[vertex] == false) { // vertex has not been discovered
					Q.push(vertex);
					discovered[vertex] = true;  // vertex has been discovered
					discovered_edges.insert(discovered_edges.end(), { v,vertex }); // the discovered_edges in order
					//cout << "discovered_edges(" << v << "," << *ai << ")" << endl;
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int vertex = it2->first;
				if (discovered[vertex] == false) { // vertex has not been discovered
					Q.push(vertex);
					discovered[vertex] = true;  // vertex has been discovered
					discovered_edges.insert(discovered_edges.end(), { v,vertex }); // the discovered_edges in order
					//cout << "discovered_edges(" << v << "," << *ai << ")" << endl;
				}
			}
		}

	}

	return discovered_edges;
}