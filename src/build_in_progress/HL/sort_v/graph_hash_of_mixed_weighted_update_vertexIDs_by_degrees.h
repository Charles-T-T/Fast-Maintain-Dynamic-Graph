#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted_update_vertexIDs.h>


bool compare_pair_second_large_to_small(const pair<int, int>& i, pair<int, int>& j)
{
	/*< is nearly 10 times slower than >*/
	return i.second > j.second;  // < is from small to big; > is from big to small.  sort by the second item of pair<int, int>
}

graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_update_vertexIDs_by_degrees_large_to_small(graph_hash_of_mixed_weighted& input_graph) {

	vector<pair<int, int>> sorted_vertices;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		sorted_vertices.push_back({ it->first, input_graph.degree(it->first) });
	}
	sort(sorted_vertices.begin(), sorted_vertices.end(), compare_pair_second_large_to_small);
	unordered_map<int, int> vertexID_old_to_new;
	int N = sorted_vertices.size();
	for (int i = 0; i < N; i++) {
		vertexID_old_to_new[sorted_vertices[i].first] = i;
	}

	return graph_hash_of_mixed_weighted_update_vertexIDs(input_graph, vertexID_old_to_new);

}

