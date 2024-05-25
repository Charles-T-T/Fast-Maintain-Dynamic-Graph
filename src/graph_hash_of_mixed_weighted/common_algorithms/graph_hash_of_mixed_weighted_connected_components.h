#pragma once
#include <queue>
#include <vector>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

std::list<std::list<int>> graph_hash_of_mixed_weighted_connected_components(graph_hash_of_mixed_weighted& input_graph) {

	/*this is to find connected_components using depth first search; time complexity O(|V|+|E|);
	related content: https://www.boost.org/doc/libs/1_68_0/boost/graph/connected_components.hpp
	https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm*/

	std::list<std::list<int>> components;

	/*time complexity: O(V)*/
	std::unordered_map<int, bool> discovered;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		discovered[it->first] = false;
	}
	//std::vector<bool> discovered(N, false); // this is wrong, as the vertices may not be 1, ..., N-1

	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		int i = it->first;
		if (discovered[i] == false) {

			std::list<int> component;
			/*below is a deppth first search; time complexity O(|V|+|E|)*/
			std::queue<int> Q; // Queue is a data structure designed to operate in FIFO (First in First out) context.
			Q.push(i);
			component.push_back(i);
			discovered[i] = true;
			while (Q.size() > 0) {
				int v = Q.front();
				Q.pop(); //Removing that vertex from queue,whose neighbour will be visited now

				auto search = input_graph.hash_of_hashs.find(v);
				if (search != input_graph.hash_of_hashs.end()) {
					for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
						int adj_v = it2->first;
						if (discovered[adj_v] == false) {
							Q.push(adj_v);
							component.push_back(adj_v);
							discovered[adj_v] = true;
						}
					}
				}
				else {
					auto search2 = input_graph.hash_of_vectors.find(v);
					for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
						int adj_v = it2->first;
						if (discovered[adj_v] == false) {
							Q.push(adj_v);
							component.push_back(adj_v);
							discovered[adj_v] = true;
						}
					}
				}
			}

			components.push_back(component);

		}
	}

	return components;

}



//std::list<std::list<int>> cpns = graph_hash_of_mixed_weighted_connected_components(input_graph);
//cout << "cpns num: " << cpns.size() << endl;
//int max_cpn_size = 0;
//for (auto it = cpns.begin(); it != cpns.end(); it++) {
//	if (max_cpn_size < (*it).size()) {
//		max_cpn_size = (*it).size();
//	}
//}
//cout << "max_cpn_size: " << max_cpn_size << endl;


std::vector<std::vector<int>> graph_hash_of_mixed_weighted_connected_components_vector_format(graph_hash_of_mixed_weighted& input_graph, int max_V_id) {

	/*this is to find connected_components using depth first search; time complexity O(|V|+|E|);
	related content: https://www.boost.org/doc/libs/1_68_0/boost/graph/connected_components.hpp
	https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm*/

	std::vector<std::vector<int>> components;

	/*time complexity: O(V)*/
	std::vector<bool> discovered((int)(max_V_id + (int)1), false);

	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		int i = it->first;

		if (discovered[i] == false) { // if memory overflow here, then the input max_V_id is too small

			std::vector<int> component;

			/*below is a breadth first search; time complexity O(|V|+|E|)*/
			std::queue<int> Q; // Queue is a data structure designed to operate in FIFO (First in First out) context.
			Q.push(i);
			component.push_back(i);
			discovered[i] = true;
			while (Q.size() > 0) {
				int v = Q.front();
				Q.pop(); //Removing that vertex from queue,whose neighbour will be visited now

				auto search = input_graph.hash_of_hashs.find(v);
				if (search != input_graph.hash_of_hashs.end()) {
					for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
						int adj_v = it2->first;
						if (discovered[adj_v] == false) {
							Q.push(adj_v);
							component.push_back(adj_v);
							discovered[adj_v] = true;
						}
					}
				}
				else {
					auto search2 = input_graph.hash_of_vectors.find(v);
					for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
						int adj_v = it2->first;
						if (discovered[adj_v] == false) {
							Q.push(adj_v);
							component.push_back(adj_v);
							discovered[adj_v] = true;
						}
					}
				}
			}

			components.push_back(component);

		}
	}

	return components;

}