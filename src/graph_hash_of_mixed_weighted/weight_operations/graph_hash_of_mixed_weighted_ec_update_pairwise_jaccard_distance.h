#pragma once
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

void graph_hash_of_mixed_weighted_ec_update_pairwise_jaccard_distance(graph_hash_of_mixed_weighted& input_graph) {

	/*time complexity: O(|V|+|E|)*/

	/*for introducing this distance: https://en.wikipedia.org/wiki/Jaccard_index ;
	The Jaccard distance, which measures dissimilarity between sample sets, is complementary
	to the Jaccard coefficient and is obtained by subtracting the Jaccard coefficient from 1;
	dist(i,j) = 1 - |V_i \cap V_j| / |V_i \cup V_j|, where V_i is the set of adjacent vertices of i;
	this is different from that in T Lappse's 2009 paper */

	/*time complexity: O(|V|+|E|)*/
	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;

		auto search = input_graph.hash_of_hashs.find(i);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					/*update_pairwise_Jaccard_distance for edge (i,j)*/
					int V_i_cap_V_j = 0;
					auto search3 = input_graph.hash_of_hashs.find(i);
					if (search3 != input_graph.hash_of_hashs.end()) {
						for (auto it3 = search3->second.begin(); it3 != search3->second.end(); it3++) {
							int k = it3->first;
							if (graph_hash_of_mixed_weighted_contain_edge(input_graph, j, k)) { // k is also adjacenct to j
								V_i_cap_V_j++;
							}
						}
					}
					else {
						auto search4 = input_graph.hash_of_vectors.find(i);
						for (auto it2 = search4->second.adj_vertices.begin(); it2 != search4->second.adj_vertices.end(); it2++) {
							int k = it2->first;
							if (graph_hash_of_mixed_weighted_contain_edge(input_graph, j, k)) { // k is also adjacenct to j
								V_i_cap_V_j++;
							}
						}
					}
					int V_i_cup_V_j = graph_hash_of_mixed_weighted_degree(input_graph, i) + graph_hash_of_mixed_weighted_degree(input_graph, j) - V_i_cap_V_j;
					double ec = 1 - (double)V_i_cap_V_j / V_i_cup_V_j;
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, ec);// update_pairwise_jaccard_distance for edge (j,i)
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) { // edge (i,j)
					/*update_pairwise_Jaccard_distance for edge (i,j)*/
					int V_i_cap_V_j = 0;
					auto search3 = input_graph.hash_of_hashs.find(i);
					if (search3 != input_graph.hash_of_hashs.end()) {
						for (auto it3 = search3->second.begin(); it3 != search3->second.end(); it3++) {
							int k = it3->first;
							if (graph_hash_of_mixed_weighted_contain_edge(input_graph, j, k)) { // k is also adjacenct to j
								V_i_cap_V_j++;
							}
						}
					}
					else {
						auto search4 = input_graph.hash_of_vectors.find(i);
						for (auto it2 = search4->second.adj_vertices.begin(); it2 != search4->second.adj_vertices.end(); it2++) {
							int k = it2->first;
							if (graph_hash_of_mixed_weighted_contain_edge(input_graph, j, k)) { // k is also adjacenct to j
								V_i_cap_V_j++;
							}
						}
					}
					int V_i_cup_V_j = graph_hash_of_mixed_weighted_degree(input_graph, i) + graph_hash_of_mixed_weighted_degree(input_graph, j) - V_i_cap_V_j;
					double ec = 1 - (double)V_i_cap_V_j / V_i_cup_V_j;
					graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, ec);// update_pairwise_jaccard_distance for edge (j,i)
				}
			}
		}
	}

}


void graph_hash_of_mixed_weighted_ec_update_pairwise_jaccard_distance_fast(graph_hash_of_mixed_weighted& input_graph, int max_N) {

	/* this function is 10 times faster than graph_hash_of_mixed_weighted_ec_update_pairwise_jaccard_distance for dense graphs,
	since this function uses merge-join to fast compute V_i_cap_V_j */


	std::vector<std::vector<std::pair<int, double>>> adj_lists(max_N + 1); // sorted lists
	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		adj_lists[it1->first] = input_graph.adj_v_and_ec(it1->first);
	}

	/*time complexity: O(|V|+|E|)*/
	for (auto it1 = input_graph.hash_of_vectors.begin(); it1 != input_graph.hash_of_vectors.end(); it1++) {
		int i = it1->first;
		for (auto it2 = adj_lists[i].begin(); it2 != adj_lists[i].end(); it2++) {
			int j = it2->first;
			if (i < j) { // edge (i,j)
				/*update_pairwise_Jaccard_distance for edge (i,j)*/
				int V_i_cap_V_j = 0;

				/*merge join style compare*/
				auto vector_i_check_pointer = adj_lists[i].begin();
				auto vector_j_check_pointer = adj_lists[j].begin();
				auto pointer_vector_i_end = adj_lists[i].end();
				auto pointer_vector_j_end = adj_lists[j].end();
				while (vector_i_check_pointer != pointer_vector_i_end && vector_j_check_pointer != pointer_vector_j_end) {
					if (vector_i_check_pointer->first == vector_j_check_pointer->first) {
						V_i_cap_V_j++;
						vector_i_check_pointer++;
					}
					else if (vector_i_check_pointer->first > vector_j_check_pointer->first) {
						vector_j_check_pointer++;
					}
					else {
						vector_i_check_pointer++;
					}
				}

				int V_i_cup_V_j = adj_lists[i].size() + adj_lists[j].size() - V_i_cap_V_j;
				double ec = 1 - (double)V_i_cap_V_j / V_i_cup_V_j;
				graph_hash_of_mixed_weighted_add_edge(input_graph, i, j, ec);// update_pairwise_jaccard_distance for edge (j,i)
			}
		}
	}

}









/*
------------------
#include <graph_hash_of_mixed_weighted/weight_operations/graph_hash_of_mixed_weighted_ec_update_pairwise_jaccard_distance.h>

int main()
{
	test_pairwise_jaccard_distance();
}
---------------------
*/


#pragma region

#include<chrono>
#include<graph_hash_of_mixed_weighted/random_graph/graph_hash_of_mixed_weighted_generate_random_connected_graph.h>
#include<graph_hash_of_mixed_weighted/two_graphs_operations/graph_hash_of_mixed_weighted_graph1_is_graph2.h>

void test_pairwise_jaccard_distance() {

	/*parameters*/
	int iteration_times = 10;
	int V = 500, E = 100000, precision = 3;
	double ec_min = 0.001, ec_max = 1;

	double time_old = 0, time_new = 0;

	/*iteration*/
	for (int i = 0; i < iteration_times; i++) {

		graph_hash_of_mixed_weighted instance_graph = graph_hash_of_mixed_weighted_generate_random_connected_graph(V, E, 0, 0, ec_min, ec_max, precision);

		graph_hash_of_mixed_weighted graph1, graph2;


		if (1) {
			auto begin = std::chrono::high_resolution_clock::now();
			graph_hash_of_mixed_weighted_ec_update_pairwise_jaccard_distance(instance_graph);
			auto end = std::chrono::high_resolution_clock::now();
			double runningtime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1e9; // s
			time_old = time_old + (double)runningtime / iteration_times;
			graph1 = instance_graph;
		}

		if (1) {
			auto begin = std::chrono::high_resolution_clock::now();
			graph_hash_of_mixed_weighted_ec_update_pairwise_jaccard_distance_fast(instance_graph, V + 1);
			auto end = std::chrono::high_resolution_clock::now();
			double runningtime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1e9; // s
			time_new = time_new + (double)runningtime / iteration_times;
			graph2 = instance_graph;
		}

		if (graph_hash_of_mixed_weighted_graph1_is_graph2(graph1, graph2) == false) {
			std::cout << "graph_hash_of_mixed_weighted_graph1_is_in_graph2(graph1, graph2) == false!" << std::endl;
			exit(1);
		}


	}

	std::cout << "time_old = " << time_old << "s" << std::endl;
	std::cout << "time_new = " << time_new << "s" << std::endl;
}
#pragma endregion test_pairwise_jaccard_distance


