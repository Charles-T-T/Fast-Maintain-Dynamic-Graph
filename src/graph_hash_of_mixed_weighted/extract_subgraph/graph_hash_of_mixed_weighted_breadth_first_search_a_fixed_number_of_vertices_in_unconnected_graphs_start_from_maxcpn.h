#pragma once


/* this function uses breadth_first_search to find a fixed number of vertices that connect to root,
including root; time complexity: O(V + E);
note that,if one root is not enough to span a fixed number of vertices, then multiple roots are randomly selected;

if target_V > |V| of input_graph, then it returns all vertices in input_graph*/

#include <graph_hash_of_mixed_weighted/extract_subgraph/graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_number_of_vertices.h>

#include <random>
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <boost/random.hpp>
#include <graph_hash_of_mixed_weighted/common_algorithms/graph_hash_of_mixed_weighted_connected_components.h>



bool compare_graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_number_of_vertices_in_unconnected_graphs_start_from_maxcpn(const std::pair<std::list<int>, int>& i, std::pair<std::list<int>, int>& j)
{
	return i.second > j.second;  // < is from small to big; > is from big to small.  sort by the second item of pair<int, int>
}



std::unordered_set<int> graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_number_of_vertices_in_unconnected_graphs_start_from_maxcpn
(graph_hash_of_mixed_weighted& input_graph, int target_V) {

	std::unordered_set<int> unselected_vertices;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); ++it) {
		int v = it->first;
		unselected_vertices.insert(v);
	}



	std::list<std::list<int>> old_cpn = graph_hash_of_mixed_weighted_connected_components(input_graph);

	std::vector<std::pair<std::list<int>, int>> sorted_vector;
	for (auto it = old_cpn.begin(); it != old_cpn.end(); it++) {
		sorted_vector.push_back({ *it , (*it).size() }); // sort cpn by sizes, from large to small
	}
	sort(sorted_vector.begin(), sorted_vector.end(), compare_graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_number_of_vertices_in_unconnected_graphs_start_from_maxcpn);
	std::list<std::list<int>> cpn;
	for (int i = 0; i < sorted_vector.size(); i++) {
		cpn.push_back(sorted_vector[i].first);
	}

	std::unordered_set<int> selected_vertices;
	while (selected_vertices.size() < target_V && unselected_vertices.size() > 0) {

		int lack_num = target_V - selected_vertices.size();

		if (cpn.front().size() >= lack_num) { //random bfs in cpn.front()

			std::time_t now_983 = std::time(0);
			boost::random::mt19937 gen_983{ static_cast<std::uint32_t>(now_983) };
			boost::random::uniform_int_distribution<> dist_983{ 0, int(cpn.front().size() - 1) };
			int start_id = dist_983(gen_983);
			int root = 0;
			int xx = 0;
			for (auto it = cpn.front().begin(); it != cpn.front().end(); it++) {
				if (xx == start_id) {
					root = *it; // an unslected root
					break;
				}
				else {
					xx++;
				}
			}

			std::unordered_set<int> hashV = graph_hash_of_mixed_weighted_breadth_first_search_a_fixed_number_of_vertices(input_graph, lack_num, root);

			for (auto it = hashV.begin(); it != hashV.end(); it++) {
				int v = *it;
				unselected_vertices.erase(v);
				selected_vertices.insert(v);
			}

		}
		else { // push cpn.front() into selected_vertices, and then remove cpn.front()

			for (auto it = cpn.front().begin(); it != cpn.front().end(); it++) {
				int v = *it;
				unselected_vertices.erase(v);
				selected_vertices.insert(v);
			}

			cpn.pop_front();

		}

	}

	return selected_vertices;
}

