#pragma once
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

std::unordered_set<int> graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_depth_of_vertices_from_one_root(graph_hash_of_mixed_weighted& input_graph, int root, int depth) {

	/*the depth of root is 0, only search depth_v <= depth*/

	std::unordered_set<int> dfs_searched_nearby;
	std::unordered_map<int, int> touched_vertices_depths; // V_ID, depth
	std::queue<int> Q_touched_not_searched_nearby;

	Q_touched_not_searched_nearby.push(root);
	touched_vertices_depths[root] = 0; // the depth of root is 0


	while (Q_touched_not_searched_nearby.size() > 0) {
		int v = Q_touched_not_searched_nearby.front();
		int v_depth = touched_vertices_depths[v];

		dfs_searched_nearby.insert(v);
		Q_touched_not_searched_nearby.pop(); //Removing v from queue,whose neighbour will be visited now

		/*processing all the neighbours of v*/
		auto search = input_graph.hash_of_hashs.find(v);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int adj_v = it2->first;
				if (touched_vertices_depths.count(adj_v) == 0 && v_depth + 1 <= depth) { // vertex has not been touched
					Q_touched_not_searched_nearby.push(adj_v);
					touched_vertices_depths[adj_v] = v_depth + 1;
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int adj_v = it2->first;
				if (touched_vertices_depths.count(adj_v) == 0 && v_depth + 1 <= depth) { // vertex has not been touched
					Q_touched_not_searched_nearby.push(adj_v);
					touched_vertices_depths[adj_v] = v_depth + 1;
				}
			}
		}

	}

	return dfs_searched_nearby;

}


std::unordered_set<int> graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_depth_of_vertices_from_multiple_roots(graph_hash_of_mixed_weighted& input_graph, std::unordered_set<int> roots, int depth) {

	/*the depth of root is 0, only search depth_v <= depth*/

	std::unordered_set<int> results;

	for (auto it = roots.begin(); it != roots.end(); it++) {
		int root = *it;
		std::unordered_set<int> searched_vertices = graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_depth_of_vertices_from_one_root(input_graph, root, depth);
		for (auto it2 = searched_vertices.begin(); it2 != searched_vertices.end(); it2++) {
			results.insert(*it2);
		}
	}

	return results;

}