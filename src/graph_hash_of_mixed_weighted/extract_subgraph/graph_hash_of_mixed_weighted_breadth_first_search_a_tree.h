#pragma once

#include <graph_hash_of_mixed_weighted/extract_subgraph/graph_hash_of_mixed_weighted_breadth_first_search_a_tree_of_edges.h>

graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_breadth_first_search_a_tree(graph_hash_of_mixed_weighted& input_graph, int& root) {

	std::vector<std::pair<int, int>> bfs_edges = graph_hash_of_mixed_weighted_breadth_first_search_a_tree_of_edges(input_graph, root);

	graph_hash_of_mixed_weighted bfs_tree;

	for (auto it = bfs_edges.begin(); it != bfs_edges.end(); it++) {

		int e1 = it->first, e2 = it->second;

		double nw1 = input_graph.hash_of_vectors[e1].vertex_weight, nw2 = input_graph.hash_of_vectors[e2].vertex_weight;

		double ec = graph_hash_of_mixed_weighted_edge_weight(input_graph, e1, e2);

		graph_hash_of_mixed_weighted_add_vertex(bfs_tree, e1, nw1);
		graph_hash_of_mixed_weighted_add_vertex(bfs_tree, e2, nw2);
		graph_hash_of_mixed_weighted_add_edge(bfs_tree, e1, e2, ec);

	}

	double nw1 = input_graph.hash_of_vectors[root].vertex_weight;
	graph_hash_of_mixed_weighted_add_vertex(bfs_tree, root, nw1);

	return bfs_tree; // bfs_tree has same weights with input graph, and may only contain one vertex
}