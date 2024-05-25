#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h> 
#include <fstream>

void graph_hash_of_mixed_weighted_save_for_GSTP(std::string instance_name,
	graph_hash_of_mixed_weighted& input_graph, graph_hash_of_mixed_weighted& group_graph,
	std::unordered_set<int>& group_vertices, double lambda) {

	std::ofstream outputFile;
	outputFile.precision(10);
	outputFile.setf(std::ios::fixed);
	outputFile.setf(std::ios::showpoint);
	outputFile.open(instance_name);

	// comments
	outputFile << "SECTION Comments" << std::endl;
	outputFile << "Name \"" << instance_name << "\"" << std::endl;
	outputFile << "Creator \"graph_hash_of_mixed_weighted_save_for_GSTP\"" << std::endl;
	outputFile << "END" << std::endl;
	outputFile << std::endl;

	// lambda
	outputFile << "lambda " << lambda << '\n';
	outputFile << std::endl;

	// input_graph
	outputFile << "input_graph |V|=" << graph_hash_of_mixed_weighted_num_vertices(input_graph) <<
		" |E|=" << graph_hash_of_mixed_weighted_num_edges(input_graph) << std::endl;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); ++it) {
		outputFile << "input_graph Vertex " << it->first << " " << it->second.vertex_weight << '\n';
	}
	outputFile << std::endl;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); ++it) {
		int i = it->first;

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				double ec = it2->second;
				if (i <= j) {
					outputFile << "input_graph Edge " << i << " " << j << " " << ec << '\n';
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				double ec = it2->second;
				if (i <= j) {
					outputFile << "input_graph Edge " << i << " " << j << " " << ec << '\n';
				}
			}
		}
	}
	outputFile << std::endl;

	// group_graph
	outputFile << "group_graph |V|=" << graph_hash_of_mixed_weighted_num_vertices(group_graph) <<
		" |E|=" << graph_hash_of_mixed_weighted_num_edges(group_graph) << std::endl;
	for (auto it = group_graph.hash_of_vectors.begin(); it != group_graph.hash_of_vectors.end(); ++it) {
		outputFile << "group_graph Vertex " << it->first << " " << it->second.vertex_weight << '\n';
	}
	outputFile << std::endl;
	for (auto it = group_graph.hash_of_vectors.begin(); it != group_graph.hash_of_vectors.end(); ++it) {
		int i = it->first;

		auto search = group_graph.hash_of_hashs.find(i);
		if (search != group_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				double ec = it2->second;
				if (i <= j) {
					outputFile << "group_graph Edge " << i << " " << j << " " << ec << '\n';
				}
			}
		}
		else {
			auto search2 = group_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				double ec = it2->second;
				if (i <= j) {
					outputFile << "group_graph Edge " << i << " " << j << " " << ec << '\n';
				}
			}
		}
	}
	outputFile << std::endl;

	// group_vertices
	outputFile << "|T|=" << group_vertices.size() << std::endl;
	for (auto it = group_vertices.begin(); it != group_vertices.end(); ++it) {
		outputFile << "group_vertices " << *it << '\n';
	}
	outputFile << std::endl;

	outputFile << "EOF" << std::endl;

}
