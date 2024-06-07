#pragma once

#include "./PLL_dynamic.h"
#include "DuQueue.h"


void WeightDecreaseMaintenance_improv_step1(
		int v1, int v2, 
		weightTYPE w_new, 
		vector<vector<two_hop_label_v1>> *L, 
		PPR_type *PPR, 
		std::vector<affected_label> *CL, 
		ThreadPool &pool_dynamic, 
		std::vector<std::future<int>> &results_dynamic) {
	// v1 --> 论文伪代码中的a
	// v2 --> 论文伪代码中的b
	for (int sl = 0; sl < 2; sl++)
	{
		if (sl == 1)
		{
			swap(v1, v2);
		}
		for (auto it : (*L)[v1])
		{
			// it.vertex --> 论文伪代码中的v
			// v2 --> 论文伪代码中的b
			if (it.vertex <= v2)
			{
				results_dynamic.emplace_back(pool_dynamic.enqueue([it, v2, L, PPR, w_new, CL]
																  {
					// query_result is {distance, common hub}
					auto query_result = 
						graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, it.vertex, v2); 

					if (query_result.first > it.distance + w_new)
					{ 
						mtx_595_1.lock();
						CL->push_back(affected_label{ v2 , it.vertex, it.distance + w_new });
						mtx_595_1.unlock();
					}
					else { 
						auto search_result = search_sorted_two_hop_label((*L)[v2], it.vertex);
						if (search_result > it.distance + w_new && search_result != MAX_VALUE) {
							mtx_595_1.lock();
							CL->push_back(affected_label{ v2, it.vertex, it.distance + w_new });
							mtx_595_1.unlock();
						}

						// 更新PPR
						if (query_result.second != it.vertex) 
						{
							mtx_5952[v2].lock();
							PPR_insert(*PPR, v2, query_result.second, it.vertex);
							mtx_5952[v2].unlock();
						}
						if (query_result.second != v2) {
							mtx_5952[it.vertex].lock();
							PPR_insert(*PPR, it.vertex, query_result.second, v2);
							mtx_5952[it.vertex].unlock();
						}
					}

					return 1; }));
			}
		}
	}

	for (auto &&result : results_dynamic)
	{
		result.get();
	}
	std::vector<std::future<int>>().swap(results_dynamic);
}

void DIFFUSE(
	graph_v_of_v_idealID &instance_graph,
	std::vector<std::vector<two_hop_label_v1>> *L,
	PPR_type *PPR, std::vector<affected_label> &CL,
	ThreadPool &pool_dynamic, std::vector<std::future<int>> &results_dynamic)
{
	// TODO 代码填空1
	// TODO debug line
	// std::cout << "DIFFUSE" << std::endl;

	if (0){ // 多线程
		// 4: for each (u, v, du) ∈ CL do
		for (const auto &cl_elem : CL)
		{
			// 线程开始
			results_dynamic.emplace_back(pool_dynamic.enqueue([&instance_graph, L, PPR, cl_elem]
															{
			int u = cl_elem.first;
			int v = cl_elem.second;
			weightTYPE du = cl_elem.dis;
			
			// 5: Dis[u] = du, Dis[s] = -1 ∀s ∈ V \ u, Q = {(u | du)}
			std::vector<weightTYPE> Dis(instance_graph.size(), -1);
			Dis[u] = du;
			DuQueue Q;
			Q.emplace(u, du);

			// 6: while Q ≠ ∅ do
			while (!Q.empty())
			{
				// 7: Pop (x | dx) out of Q
				std::pair<int, weightTYPE> topElem = Q.top();
				Q.pop();
				int x = topElem.first;
				weightTYPE dx = topElem.second;

				// 7: L(x)[v] = dx
				{
					mtx_595_2.lock();
					insert_sorted_two_hop_label((*L)[x], v, dx);
					mtx_595_2.unlock();
				}

				// 8: for each xn ∈ N(x) 
				for (int i = 0; i < instance_graph[x].size(); ++i)
				{
					int xn = instance_graph[x][i].first;
					weightTYPE w_xn_x = instance_graph[x][i].second;

					// 8: such that r(v) > r(xn) do
					if (v <= xn)
					{
						int h_c;
						// 9: if Dis[xn] == -1 then Dis[xn] = Query(xn, v, L)
						if (Dis[xn] == -1)
						{
							auto query_result = 
							graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, xn, v);
							Dis[xn] = query_result.first;
							h_c = query_result.second;
						}

						// 10: if Dis[xn] > dx + w(xn, x) then
						if (Dis[xn] > dx + w_xn_x)
						{
							// 11: Dis[xn] = dx + w(xn, n);
							Dis[xn] = dx + w_xn_x;
							h_c = v;
							// 11: Insert (or update) (xn | Dis[xn]) ∈ Q
							Q.emplace(xn, Dis[xn]);
						}

						// 12: else
						else
						{
							weightTYPE search_result = search_sorted_two_hop_label((*L)[xn], v);
							// 13: if v ∈ C(xn) & min{L(xn)[v], Q(xn)} > dnew = dx + w(xn, x) then
							if (std::min(search_result, Q[xn]) > dx + w_xn_x && search_result != MAX_VALUE)
							{
								// 14: Insert (or update) (xn | dnew) ∈	Q
								Q.emplace(xn, dx + w_xn_x);
							}
 
							// 15: PPR[xn, hc].push(v)
							{
								mtx_5952[xn].lock();
								PPR_insert(*PPR, xn, h_c, v);
								mtx_5952[xn].unlock();
							}
							// 15: PPR[v, hv].push(xn)
							{
								mtx_5952[v].lock();
								PPR_insert(*PPR, v, h_c, xn);
								mtx_5952[v].unlock();
							}
						}
					}
				}
			}
				return 1; })); // 线程结束
		}

		// 等待所有线程结束
		for (auto &&result : results_dynamic)
		{
			result.get();
		}
		std::vector<std::future<int>>().swap(results_dynamic);
	} // 多线程
	

	else { // 非多线程
		for (const auto &cl_elem : CL) {
			int u = cl_elem.first;
			int v = cl_elem.second;
			weightTYPE du = cl_elem.dis;
			
			// 5: Dis[u] = du, Dis[s] = -1 ∀s ∈ V \ u, Q = {(u | du)}
			std::vector<weightTYPE> Dis(instance_graph.size(), -1);
			Dis[u] = du;
			DuQueue Q;
			Q.emplace(u, du);

			// 6: while Q ≠ ∅ do
			while (!Q.empty())
			{
				// 7: Pop (x | dx) out of Q
				std::pair<int, weightTYPE> topElem = Q.top();
				Q.pop();
				int x = topElem.first;
				weightTYPE dx = topElem.second;

				// 7: L(x)[v] = dx
				{
					mtx_595_1.lock();
					insert_sorted_two_hop_label((*L)[x], v, dx);
					mtx_595_1.unlock();
				}

				// 8: for each xn ∈ N(x) 
				for (int i = 0; i < instance_graph[x].size(); ++i)
				{
					int xn = instance_graph[x][i].first;
					weightTYPE w_xn_x = instance_graph[x][i].second;

					// 8: such that r(v) > r(xn) do
					if (v <= xn)
					{
						int h_c;
						// 9: if Dis[xn] == -1 then Dis[xn] = Query(xn, v, L)
						if (Dis[xn] == -1)
						{
							auto query_result = 
								graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, xn, v);
							Dis[xn] = query_result.first;
							h_c = query_result.second;
						}

						// 10: if Dis[xn] > dx + w(xn, x) then
						if (Dis[xn] > dx + w_xn_x)
						{
							// 11: Dis[xn] = dx + w(xn, n);
							Dis[xn] = dx + w_xn_x;
							h_c = v;
							// 11: Insert (or update) (xn | Dis[xn]) ∈ Q
							Q.emplace(xn, Dis[xn]);
						}

						// 12: else
						else
						{
							weightTYPE search_result = search_sorted_two_hop_label((*L)[xn], v);
							// 13: if v ∈ C(xn) & min{L(xn)[v], Q(xn)} > dnew = dx + w(xn, x) then
							if (std::min(search_result, Q[xn]) > dx + w_xn_x && search_result != MAX_VALUE)
							{
								// 14: Insert (or update) (xn | dnew) ∈	Q
								Q.emplace(xn, dx + w_xn_x);
							}
							// 15: PPR[xn, hc].push(v)
							{
								mtx_5952[xn].lock();
								PPR_insert(*PPR, xn, h_c, v);
								mtx_5952[xn].unlock();
							}
							// 15: PPR[v, hv].push(xn)
							{
								mtx_5952[v].lock();
								PPR_insert(*PPR, v, h_c, xn);
								mtx_5952[v].unlock();
							}
						}
					}
				} 
			} 
		} 
	} // end of DIFFUSE
} 

void WeightDecreaseMaintenance_improv(graph_v_of_v_idealID &instance_graph, graph_hash_of_mixed_weighted_two_hop_case_info_v1 &mm, int v1, int v2, weightTYPE w_old, weightTYPE w_new,
									  ThreadPool &pool_dynamic, std::vector<std::future<int>> &results_dynamic)
{

	std::vector<affected_label> CL;
	WeightDecreaseMaintenance_improv_step1(v1, v2, w_new, &mm.L, &mm.PPR, &CL, pool_dynamic, results_dynamic);

	DIFFUSE(instance_graph, &mm.L, &mm.PPR, CL, pool_dynamic, results_dynamic);
}
