#pragma once
#include <fstream>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

void graph_hash_of_mixed_weighted_save_graph_with_weight(std::string instance_name,
	graph_hash_of_mixed_weighted& input_graph, double weight) {

	std::ofstream outputFile;
	outputFile.precision(10);
	outputFile.setf(std::ios::fixed);
	outputFile.setf(std::ios::showpoint);
	outputFile.open(instance_name);

	// comments
	outputFile << "SECTION Comments" << std::endl;
	outputFile << "Name \"" << instance_name << "\"" << std::endl;
	outputFile << "Creator \"graph_hash_of_mixed_weighted_save_graph_with_regulating_weight\"" << std::endl;
	outputFile << "END" << std::endl;
	outputFile << std::endl;

	// regulating_weight
	outputFile << "weight " << weight << '\n';
	outputFile << std::endl;

	// input_graph
	outputFile << "input_graph |V|=" << graph_hash_of_mixed_weighted_num_vertices(input_graph) <<
		" |E|=" << graph_hash_of_mixed_weighted_num_edges(input_graph) << std::endl;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); ++it) {
		outputFile << "Vertex " << it->first << " " << it->second.vertex_weight << '\n';
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
					outputFile << "Edge " << i << " " << j << " " << ec << '\n';
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				double ec = it2->second;
				if (i <= j) {
					outputFile << "Edge " << i << " " << j << " " << ec << '\n';
				}
			}
		}
	}
	outputFile << std::endl;

	outputFile << "EOF" << std::endl;

}
