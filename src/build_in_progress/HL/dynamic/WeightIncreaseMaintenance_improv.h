#pragma once

#include <build_in_progress/HL/dynamic/PLL_dynamic.h>
#include "DuQueue.h"

void SPREAD1(graph_v_of_v_idealID &instance_graph, vector<vector<two_hop_label_v1>> *L,
			 std::vector<affected_label> &al1, std::vector<pair_label> *al2, ThreadPool &pool_dynamic, std::vector<std::future<int>> &results_dynamic)
{

	/*TO DO 2*/
	// TODO 2
}

void SPREAD2(graph_v_of_v_idealID &instance_graph, vector<vector<two_hop_label_v1>> *L, PPR_type *PPR,
			 std::vector<pair_label> &al2, std::vector<affected_label> *al3, ThreadPool &pool_dynamic, std::vector<std::future<int>> &results_dynamic)
{

	/*TO DO 3*/
	// TODO 3
}

void SPREAD3(graph_v_of_v_idealID &instance_graph,
			 vector<vector<two_hop_label_v1>> *L, PPR_type *PPR,
			 std::vector<affected_label> &al3,
			 ThreadPool &pool_dynamic, std::vector<std::future<int>> &results_dynamic)
{
	/*TO DO 4*/
	// TODO 4

	for (const auto &al3_elem : al3)
	{
		// 线程开始
		results_dynamic.emplace_back(pool_dynamic.enqueue([&instance_graph, L, PPR, al3_elem]
														  {
			// 24: for each (u, v, du) ∈ AL3 do
			int u = al3_elem.first;
			int v = al3_elem.second;
			weightTYPE du = al3_elem.dis;
			
			auto query_result = graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, u, v);

			// 25: if Q(u, v, L) ≤ du
			if (query_result.first <= du){
				// 25: then PPR[u, hc].push(v)
				{
					mtx_5952[u].lock();
					PPR_insert(*PPR, u, query_result.second, v);
					mtx_5952[u].unlock();
				}
				// 25: PPR[v, hc].push(u)
				{
					mtx_5952[v].lock();
					PPR_insert(*PPR, v, query_result.second, u);
					mtx_5952[v].unlock();
				}
				// 25: Continue
				// TODO 这里应该是直接结束线程？
				return 1;
			}

			// 26: Dis[u] = du, Dis[s] = -1 for each s ∈ V \ u, Q = {(u | du)}
			std::vector<weightTYPE> Dis(instance_graph.size(), -1);
			Dis[u] = du;
			DuQueue Q;
			Q.emplace(u, du);

			// 27: while Q ≠ ∅ do
			while (!Q.empty())
			{ 
				// 28: Pop (x | dx) out of Q
				std::pair<weightTYPE, int> topElem = Q.top();
				Q.pop();
				int x = topElem.first;
				weightTYPE dx = topElem.second;

				// 28: L(x)[v] = min(dx, L(x)[v])
				{
					mtx_595_1.lock();
					weightTYPE minLen = std::min(search_sorted_two_hop_label((*L)[x], v), dx);
					insert_sorted_two_hop_label((*L)[x], v, minLen);
					mtx_595_1.unlock();
				}
 
				// 29: for each xn ∈ N(x)
				for (int i = 0; i < instance_graph[x].size(); ++i)
				{
					int xn = instance_graph[x][i].first;
					weightTYPE w_xn_x = instance_graph[x][i].second;

					// 29: such that r(v) > r(xn) do
					if (v <= xn)
					{
						int h_c;
						// 30: if Dis[xn] == -1 then Dis[xn] = Query(xn, v, L)
						if (Dis[xn] == -1)
						{
							auto query_result = graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, xn, v);
							Dis[xn] = query_result.first;
							h_c = query_result.second;
						}

						// 31: if Dis[xn] > dx + w(xn, x) then
						if (Dis[xn] > dx + w_xn_x)
						{ 
							// 32: Dis[xn] = dx + w(xn, x)
							Dis[xn] = dx + w_xn_x;
							h_c = v;
							// 32: Insert (or update) (xn | Dis[xn]) ∈ Q
							Q.emplace(xn, Dis[xn]);
						}

						// 33: else
						else
						{
							weightTYPE search_result = search_sorted_two_hop_label((*L)[xn], v);
							if (search_result > dx + w_xn_x && search_result != MAX_VALUE)
							{
								Q.emplace(xn, dx + w_xn_x);
							}
							// 33: PPR[xn, hc].push(v)
							{
								mtx_5952[xn].lock();
								PPR_insert(*PPR, xn, h_c, v);
								mtx_5952[xn].unlock();
							}
							// 33: PPR[v, hc].push(xn)
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
}

void WeightIncreaseMaintenance_improv(graph_v_of_v_idealID &instance_graph, graph_hash_of_mixed_weighted_two_hop_case_info_v1 &mm, int v1, int v2, weightTYPE w_old, weightTYPE w_new,
									  ThreadPool &pool_dynamic, std::vector<std::future<int>> &results_dynamic)
{

	std::vector<affected_label> al1, al3;
	std::vector<pair_label> al2;

	/*it's slow to paralize the following part*/
	for (auto it : mm.L[v1])
	{ // TODO 问题5、问题8
		if (it.vertex <= v2 && abs(search_sorted_two_hop_label(mm.L[v2], it.vertex) - it.distance - w_old) < 1e-5)
		{
			al1.push_back(affected_label(v2, it.vertex, it.distance + w_old));
		}
	}
	for (auto it : mm.L[v2])
	{
		if (it.vertex <= v1 && abs(search_sorted_two_hop_label(mm.L[v1], it.vertex) - it.distance - w_old) < 1e-5)
		{
			al1.push_back(affected_label(v1, it.vertex, it.distance + w_old));
		}
	}

	// cout << "al1.size() " << al1.size() << endl;

	SPREAD1(instance_graph, &mm.L, al1, &al2, pool_dynamic, results_dynamic);
	SPREAD2(instance_graph, &mm.L, &mm.PPR, al2, &al3, pool_dynamic, results_dynamic);
	SPREAD3(instance_graph, &mm.L, &mm.PPR, al3, pool_dynamic, results_dynamic);

	// for (auto it : al2) {
	//	cout << "al2 " << it.first << " " << it.second << endl;
	// }
	// for (auto it : al3) {
	//	cout << "al3 " << it.first << " " << it.second << " " << it.dis << endl;
	// }
}
