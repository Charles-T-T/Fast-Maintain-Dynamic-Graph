#pragma once
#include <iostream>
#include <chrono>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <graph_hash_of_mixed_weighted/two_graphs_operations/graph_hash_of_mixed_weighted_to_graph_v_of_v_idealID_2.h>
#include "./two_hop_labels_base.h"


bool PLL_dynamic_generate_PPR = true;

void PLL_thread_function_dij_mixed(int v_k, int N)
{
	/*Pruned Dijkstra from vertex v_k; see Algorithm 1 in 2013 Japan SIGMOD paper*/

	if (PLL_throw_error) {
		return;
	}
	if (labal_size_595 > max_labal_size_595) {
		PLL_throw_error = true;
		throw reach_limit_error_string_MB;  // after catching error, must call terminate_procedures_595(), otherwise this PLL cannot be reused
	}
	if (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - begin_time_595).count() > max_run_time_nanoseconds_595) {
		PLL_throw_error = true;
		throw reach_limit_error_string_time;  // after catching error, must call terminate_procedures_595(), otherwise this PLL cannot be reused
	}

	mtx_595[max_N_ID_for_mtx_595 - 1].lock();
	int used_id = Qid_595.front();
	Qid_595.pop();
	mtx_595[max_N_ID_for_mtx_595 - 1].unlock();

	queue<int> P_changed_vertices, T_changed_vertices;
	auto& Q_handles = Q_handles_dij_595[used_id];

	PLL_dynamic_node_for_sp node;
	boost::heap::fibonacci_heap<PLL_dynamic_node_for_sp> Q;
	two_hop_label_v1 xx;

	node.vertex = v_k;
	node.priority_value = 0;
	Q_handles[v_k] = Q.push(node);
	P_dij_595[used_id][v_k] = 0;
	P_changed_vertices.push(v_k);

	mtx_595[v_k].lock();
	int L_v_k_size = L_temp_595[v_k].size();
	for (int i = 0; i < L_v_k_size; i++) {
		int L_v_k_i_vertex = L_temp_595[v_k][i].vertex;
		T_dij_595[used_id][L_v_k_i_vertex] = L_temp_595[v_k][i].distance; //allocate T values for L_temp_595[v_k]
		T_changed_vertices.push(L_v_k_i_vertex);
	}
	mtx_595[v_k].unlock();
	//因为v-k的标签在从自己出发的过程中不会发生改变，并且在求query的过程中每次都会用到，所以可以提前取出来放在T数组，节省后面查找的时间

	long long int new_label_num = 0;

	while (Q.size() > 0) {

		node = Q.top();
		Q.pop();
		int u = node.vertex;

		if (v_k <= u) { // this pruning condition is not in the original 2013 PLL paper
			weightTYPE P_u = node.priority_value;
			weightTYPE P_u_with_error = P_u + 1e-5;
			weightTYPE query_v_k_u = std::numeric_limits<weightTYPE>::max();
			int common_hub_for_query_v_k_u;

#ifdef _WIN32
			mtx_595[u].lock();
			auto L_u_size = L_temp_595[u].size(); // a vector<PLL_with_non_adj_reduction_sorted_label>
			mtx_595[u].unlock();
			for (int i = 0; i < L_u_size; i++) {
				mtx_595[u].lock();      // put lock in for loop is very slow, but it may be the only way under Windows
				weightTYPE dis = L_temp_595[u][i].distance + T_dij_595[used_id][L_temp_595[u][i].vertex];
				mtx_595[u].unlock();
				if (query_v_k_u > dis) { query_v_k_u = dis; common_hub_for_query_v_k_u = L_temp_595[u][i].vertex; }
			} //求query的值		
#else
			mtx_595[u].lock();
			auto L_u_size1 = L_temp_595[u].size(); // a vector<PLL_with_non_adj_reduction_sorted_label>
			for (int i = 0; i < L_u_size1; i++) {
				weightTYPE dis = L_temp_595[u][i].distance + T_dij_595[used_id][L_temp_595[u][i].vertex];   // dont know why this code does not work under Windows
				if (query_v_k_u > dis) { query_v_k_u = dis; common_hub_for_query_v_k_u = L_temp_595[u][i].vertex; }
			} //求query的值
			mtx_595[u].unlock();
#endif

			if (P_u_with_error < query_v_k_u) { // this is pruning
				xx.vertex = v_k;
				xx.distance = P_u;

				mtx_595[u].lock();
				L_temp_595[u].push_back(xx); //新增标签，并行时L_temp_595[u]里面的标签不一定是按照vertex ID排好序的，但是因为什么query时用了T_dij_595的trick，没必要让L_temp_595[u]里面的标签排好序
				mtx_595[u].unlock();
				new_label_num++;

				/*下面是dij更新邻接点的过程，同时更新优先队列和距离*/
				int u_adj_size = ideal_graph_595[u].size();
				for (int i = 0; i < u_adj_size; i++) {
					int adj_v = ideal_graph_595[u][i].first; // this needs to be locked
					weightTYPE ec = ideal_graph_595[u][i].second;

					if (P_dij_595[used_id][adj_v] == std::numeric_limits<weightTYPE>::max()) { //尚未到达的点
						node.vertex = adj_v;
						node.priority_value = P_u + ec;
						Q_handles[adj_v] = Q.push(node);
						P_dij_595[used_id][adj_v] = node.priority_value;
						P_changed_vertices.push(adj_v);
					}
					else {
						if (P_dij_595[used_id][adj_v] > P_u + ec) {
							node.vertex = adj_v;
							node.priority_value = P_u + ec;
							Q.update(Q_handles[adj_v], node);
							P_dij_595[used_id][adj_v] = node.priority_value;
						}
						//else {
						//	///* add v_k into PPR(u,common_hub_for_query_v_k_u), and add u into PPR(v_k,common_hub_for_query_v_k_u)*/
						//	//mtx_595[adj_v].lock();
						//	//PPR_insert(PPR_595, adj_v, v_k, v_k);
						//	//mtx_595[adj_v].unlock();
						//	//mtx_595[v_k].lock();
						//	//PPR_insert(PPR_595, v_k, v_k, adj_v);
						//	//mtx_595[v_k].unlock();
						//}
					}
				}
			}	
			else if (PLL_dynamic_generate_PPR) {
				/* add v_k into PPR(u,common_hub_for_query_v_k_u), and add u into PPR(v_k,common_hub_for_query_v_k_u)*/
				if (common_hub_for_query_v_k_u != v_k) {
					mtx_595[u].lock();
					PPR_insert(PPR_595, u, common_hub_for_query_v_k_u, v_k);
					mtx_595[u].unlock();
				}
				if (common_hub_for_query_v_k_u != u) {
					mtx_595[v_k].lock();
					PPR_insert(PPR_595, v_k, common_hub_for_query_v_k_u, u);
					mtx_595[v_k].unlock();
				}
			}
		}
	}

	while (P_changed_vertices.size() > 0) {
		P_dij_595[used_id][P_changed_vertices.front()] = std::numeric_limits<weightTYPE>::max(); // reverse-allocate P values
		P_changed_vertices.pop();
	}
	while (T_changed_vertices.size() > 0) {
		T_dij_595[used_id][T_changed_vertices.front()] = std::numeric_limits<weightTYPE>::max(); // reverse-allocate T values
		T_changed_vertices.pop();
	}

	mtx_595[v_k].lock();
	vector<two_hop_label_v1>(L_temp_595[v_k]).swap(L_temp_595[v_k]); // swap释放vector中多余空间： https://blog.csdn.net/qq_41929943/article/details/103190891 
	mtx_595[v_k].unlock();

	mtx_595[max_N_ID_for_mtx_595 - 1].lock();
	Qid_595.push(used_id);
	labal_size_595 = labal_size_595 + new_label_num;
	mtx_595[max_N_ID_for_mtx_595 - 1].unlock();
}

void PLL_dynamic(graph_hash_of_mixed_weighted& input_graph, int max_N_ID, int num_of_threads, graph_hash_of_mixed_weighted_two_hop_case_info_v1& case_info){
	
	//----------------------------------- step 1: initialization ------------------------------------------------------------------
	auto begin = std::chrono::high_resolution_clock::now();
	/*information prepare*/
	begin_time_595 = std::chrono::high_resolution_clock::now();
	max_run_time_nanoseconds_595 = case_info.max_run_time_seconds * 1e9;
	labal_size_595 = 0;
	max_labal_size_595 = case_info.max_labal_size;

	if (max_N_ID > max_N_ID_for_mtx_595) {
		cout << "max_N_ID > max_N_ID_for_mtx_595; max_N_ID_for_mtx_595 is too small!" << endl;
		exit(1);
	}
	mtx_595[max_N_ID_for_mtx_595 - 1].lock();
	if (this_parallel_PLL_PSL_is_running_595 == true) {
		cout << "the following parallel PLL code cannot be run parallelly, due to globel values" << endl;
		exit(1);
	}
	this_parallel_PLL_PSL_is_running_595 = true;
	PLL_throw_error = false;
	mtx_595[max_N_ID_for_mtx_595 - 1].unlock();

	L_temp_595.resize(max_N_ID);
	PPR_595.resize(max_N_ID);
	int N = input_graph.hash_of_vectors.size();
	/*change graphs*/
	ideal_graph_595 = graph_hash_of_mixed_weighted_to_graph_v_of_v_idealID_2(input_graph, max_N_ID);

	auto end = std::chrono::high_resolution_clock::now();
	case_info.time_initialization = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1e9; // s


	//----------------------------------------------- step 3: generate labels ---------------------------------------------------------------
	begin = std::chrono::high_resolution_clock::now();

	/*seaching shortest paths*/
	ThreadPool pool(num_of_threads);
	std::vector< std::future<int> > results; // return typename: xxx
	P_dij_595.resize(num_of_threads);
	T_dij_595.resize(num_of_threads);
	Q_handles_dij_595.resize(num_of_threads);
	queue<int>().swap(Qid_595);
	for (int i = 0; i < num_of_threads; i++) {
		P_dij_595[i].resize(N, std::numeric_limits<weightTYPE>::max());
		T_dij_595[i].resize(N, std::numeric_limits<weightTYPE>::max());
		Q_handles_dij_595[i].resize(N);
		Qid_595.push(i);
	}
	int push_num = 0;
	for (int v_k = 0; v_k < N; v_k++) {
		if (ideal_graph_595[v_k].size() > 0) {  // not from isolated vertices
			results.emplace_back(
				pool.enqueue([v_k, N] { // pass const type value j to thread; [] can be empty
					PLL_thread_function_dij_mixed(v_k, N);
					return 1; // return to results; the return type must be the same with results
					})
			);
			push_num++;
		}
	}
	for (auto&& result : results)
		result.get(); //all threads finish here
	results.clear();
	end = std::chrono::high_resolution_clock::now();
	case_info.time_generate_labels = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / 1e9; // s

	for (int i = 0; i < max_N_ID; i++) {
		sort(L_temp_595[i].begin(), L_temp_595[i].end(), compare_two_hop_label_small_to_large); // sort is necessary
	}

	case_info.L = L_temp_595;
	case_info.PPR = PPR_595;
	two_hop_clear_global_values();
}