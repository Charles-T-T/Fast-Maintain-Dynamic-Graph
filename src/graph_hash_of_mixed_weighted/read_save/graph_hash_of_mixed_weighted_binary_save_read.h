#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <text_mining/binary_save_read_vector_of_vectors.h>

/*
save graph as vector<vector<pair<int, double>>>; each vector is ..... <v,w_v>, <0/1,NULL> (pairs before the last two pairs is the adj list of v)
(in the last pair, 0 means that the adj list of v is in vectors in graph, whle 1 means that ~ is in hashs in graph)
*/

void graph_hash_of_mixed_weighted_binary_save(graph_hash_of_mixed_weighted& save_graph, std::string save_path) {

	std::vector<std::vector<std::pair<int, double>>> save_vectors;

	for (auto it = save_graph.hash_of_vectors.begin(); it != save_graph.hash_of_vectors.end(); it++) {
		int v = it->first;
		double w_v = it->second.vertex_weight;

		std::vector<std::pair<int, double>> save_vector;

		auto search = save_graph.hash_of_hashs.find(v);
		if (search != save_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int adj_v = it2->first;
				double ec = it2->second;
				save_vector.push_back({ adj_v, ec });
			}
			save_vector.push_back({ v, w_v });
			save_vector.push_back({ 1, NULL });
		}
		else {
			save_vector = it->second.adj_vertices;
			save_vector.push_back({ v, w_v });
			save_vector.push_back({ 0, NULL });
		}

		save_vectors.push_back(save_vector);
	}

	binary_save_vector_of_vectors(save_path, save_vectors);
}



graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_binary_read(std::string save_path) {

	std::vector<std::vector<std::pair<int, double>>> save_vectors;
	binary_read_vector_of_vectors(save_path, save_vectors);

	graph_hash_of_mixed_weighted save_graph;

	for (auto it = save_vectors.begin(); it != save_vectors.end(); it++) {

		int vector_size = (*it).size();

		save_graph.hash_of_vectors[(*it)[vector_size - 2].first].vertex_weight = (*it)[vector_size - 2].second;

		if ((*it)[vector_size - 1].first == 0) {
			if (vector_size > 2) {
				std::vector<std::pair<int, double>> adj_list((*it).begin(), (*it).begin() + vector_size - 2); // https://www.techiedelight.com/get-slice-sub-vector-from-vector-cpp/
				save_graph.hash_of_vectors[(*it)[vector_size - 2].first].adj_vertices = adj_list;
			}
		}
		else {
			std::unordered_map<int, double> adj_list_hash;
			std::vector<std::pair<int, double>> adj_list((*it).begin(), (*it).begin() + vector_size - 2); // https://www.techiedelight.com/get-slice-sub-vector-from-vector-cpp/
			for (auto it2 = adj_list.begin(); it2 != adj_list.end(); it2++) {
				adj_list_hash[it2->first] = it2->second;
			}
			save_graph.hash_of_hashs[(*it)[vector_size - 2].first] = adj_list_hash;
		}

	}

	return save_graph;
}
