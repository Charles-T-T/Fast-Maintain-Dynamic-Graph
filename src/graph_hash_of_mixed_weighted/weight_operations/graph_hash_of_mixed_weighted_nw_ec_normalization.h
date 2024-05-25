#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

void graph_hash_of_mixed_weighted_nw_ec_normalization_with_range(graph_hash_of_mixed_weighted& input_graph, double range_left, double range_right) {

	/*this is "min-max normalization" for bringing all node weights to [0,1];
	https://en.wikipedia.org/wiki/Normalization_(statistics)
	time complexity: O(|V|+|E|)*/

	double min_nw = std::numeric_limits<double>::max();
	double max_nw = std::numeric_limits<double>::min();
	double min_ec = std::numeric_limits<double>::max();
	double max_ec = std::numeric_limits<double>::min();

	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		double w_i = it1->second.vertex_weight;
		if (w_i < min_nw) {
			min_nw = w_i;
		}
		if (w_i > max_nw) {
			max_nw = w_i;
		}

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					if (c_ij < min_ec) {
						min_ec = c_ij;
					}
					if (c_ij > max_ec) {
						max_ec = c_ij;
					}
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					if (c_ij < min_ec) {
						min_ec = c_ij;
					}
					if (c_ij > max_ec) {
						max_ec = c_ij;
					}
				}
			}
		}
	}


	double range = range_right - range_left;


	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		double w_i = it1->second.vertex_weight;
		double new_w_i = (w_i - min_nw) / (max_nw - min_nw) * range + range_left;
		it1->second.vertex_weight = new_w_i;

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					double new_c_ij = (c_ij - min_ec) / (max_ec - min_ec) * range + range_left;
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, new_c_ij);
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					double new_c_ij = (c_ij - min_ec) / (max_ec - min_ec) * range + range_left;
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, new_c_ij);
				}
			}
		}
	}
}



void graph_hash_of_mixed_weighted_nw_normalization_with_range(graph_hash_of_mixed_weighted& input_graph, double range_left, double range_right) {

	/*this is "min-max normalization" for bringing all node weights to [0,1];
	https://en.wikipedia.org/wiki/Normalization_(statistics)
	time complexity: O(|V|+|E|)*/

	double min_nw = std::numeric_limits<double>::max();
	double max_nw = std::numeric_limits<double>::min();

	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		double w_i = it1->second.vertex_weight;
		if (w_i < min_nw) {
			min_nw = w_i;
		}
		if (w_i > max_nw) {
			max_nw = w_i;
		}
	}

	double range = range_right - range_left;


	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		double w_i = it1->second.vertex_weight;
		double new_w_i = (w_i - min_nw) / (max_nw - min_nw) * range + range_left;
		it1->second.vertex_weight = new_w_i;
	}
}


void graph_hash_of_mixed_weighted_ec_normalization_with_range(graph_hash_of_mixed_weighted& input_graph, double range_left, double range_right) {

	/*this is "min-max normalization" for bringing all node weights to [0,1];
	https://en.wikipedia.org/wiki/Normalization_(statistics)
	time complexity: O(|V|+|E|)*/

	double min_ec = std::numeric_limits<double>::max();
	double max_ec = std::numeric_limits<double>::min();

	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					if (c_ij < min_ec) {
						min_ec = c_ij;
					}
					if (c_ij > max_ec) {
						max_ec = c_ij;
					}
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					if (c_ij < min_ec) {
						min_ec = c_ij;
					}
					if (c_ij > max_ec) {
						max_ec = c_ij;
					}
				}
			}
		}
	}


	double range = range_right - range_left;


	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					double new_c_ij = (c_ij - min_ec) / (max_ec - min_ec) * range + range_left;
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, new_c_ij);
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					double new_c_ij = (c_ij - min_ec) / (max_ec - min_ec) * range + range_left;
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, new_c_ij);
				}
			}
		}
	}
}


void graph_hash_of_mixed_weighted_nw_ec_normalization(graph_hash_of_mixed_weighted& input_graph) {

	/*this is "min-max normalization" for bringing all node weights to [0,1];
	https://en.wikipedia.org/wiki/Normalization_(statistics)
	time complexity: O(|V|+|E|)*/

	double min_nw = std::numeric_limits<double>::max();
	double max_nw = std::numeric_limits<double>::min();
	double min_ec = std::numeric_limits<double>::max();
	double max_ec = std::numeric_limits<double>::min();

	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		double w_i = it1->second.vertex_weight;
		if (w_i < min_nw) {
			min_nw = w_i;
		}
		if (w_i > max_nw) {
			max_nw = w_i;
		}

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					if (c_ij < min_ec) {
						min_ec = c_ij;
					}
					if (c_ij > max_ec) {
						max_ec = c_ij;
					}
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					if (c_ij < min_ec) {
						min_ec = c_ij;
					}
					if (c_ij > max_ec) {
						max_ec = c_ij;
					}
				}
			}
		}
	}


	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		double w_i = it1->second.vertex_weight;
		double new_w_i = (w_i - min_nw) / (max_nw - min_nw);
		it1->second.vertex_weight = new_w_i;

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					double new_c_ij = (c_ij - min_ec) / (max_ec - min_ec);
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, new_c_ij);
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					double c_ij = it2->second;
					double new_c_ij = (c_ij - min_ec) / (max_ec - min_ec);
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, new_c_ij);
				}
			}
		}
	}
}

