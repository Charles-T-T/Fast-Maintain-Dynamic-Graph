#pragma once

#include <build_in_progress/HL/dynamic/PLL_dynamic.h>
#include "DuQueue.h"

void SPREAD1(graph_v_of_v_idealID &instance_graph, 
		vector<vector<two_hop_label_v1>> *L, 
		std::vector<affected_label> &al1, 
		std::vector<pair_label> *al2, 
		ThreadPool &pool_dynamic, 
		std::vector<std::future<int>> &results_dynamic) {
	/* TODO 2*/
	// 8: for each (u, v, d') ∈ AL1 do
	for (const auto &al : al1)
	{
		int u = al.first;
		int v = al.second;
		weightTYPE du = al.dis;

		// 9: Queue = {(u, d')}
		std::queue<pair<int, weightTYPE>> Q;
		Q.push(std::make_pair(u, du));

		// 10: while Queue not empty do
		while (!Q.empty())
		{
			// 11: Queue.pop((x, dx))
			std::pair<int, weightTYPE> topElem = Q.front();
			Q.pop();
			int x = topElem.first;
			weightTYPE dx = topElem.second;
			// 11: L(x)[v] = ∞
			{
				mtx_595_1.lock();
				insert_sorted_two_hop_label((*L)[x], v, MAX_VALUE);
				mtx_595_1.unlock();
			}
			// 11: AL2.push((x, v))
			al2->push_back(pair_label{x, v});

			// 12: for each xn ∈ N(x)
			for (int i = 0; i < instance_graph[x].size(); ++i)
			{
				int xn = instance_graph[x][i].first;
				weightTYPE w_xn_x = instance_graph[x][i].second;

				// 12: such that r(v) > r(xn) do
				if (v < xn)
				{
					// 13: if (v, dx + w(x, xn)) ∈ L(xn) then
					if (abs(search_sorted_two_hop_label((*L)[xn], v) - (dx + w_xn_x)) < 1e-5)
					{
						// 13: Queue.push((xn, dx + w(x, xn)))
						Q.push(std::make_pair(xn, dx + w_xn_x));
					}
				}
			}
		}
	}
}

void SPREAD2(
		graph_v_of_v_idealID &instance_graph, 
		vector<vector<two_hop_label_v1>> *L, 
		PPR_type *PPR,
		std::vector<pair_label> &al2, 
		std::vector<affected_label> *al3, 
		ThreadPool &pool_dynamic, 
		std::vector<std::future<int>> &results_dynamic) {
	/* TODO 3*/
	// for each (x, y) ∈ AL2 do
	for (const auto &each_pair : al2)
	{
		int x = each_pair.first;
		int y = each_pair.second;
		std::vector<int> t_vec = PPR_retrieve(*PPR, x, y);
		t_vec.push_back(y);

		// 15: for each t ∈ PPR[x, y] ∪ y do
		for (const auto &t : t_vec)
		{
			// 16: if r(t) > r(x) then
			if (t < x)
			{
				// 17: d1(x, t) = min({L(xn)[t] + w(x, xn)}), xn ∈ N(x)
				weightTYPE d1 = MAX_VALUE;
				for (int i = 0; i < instance_graph[x].size(); ++i)
				{
					int xn = instance_graph[x][i].first;
					weightTYPE w_xn_x = instance_graph[x][i].second;
					weightTYPE search_result = search_sorted_two_hop_label((*L)[xn], t);
					weightTYPE d1_new = search_result + w_xn_x;
					// update d1
					d1 = min(d1, d1_new);
				}

				// 18: if Query(x, t, L) > d1(x, t) then
				auto query_result =
					graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, x, t);
				weightTYPE query_xtL = query_result.first;
				int h_c = query_result.second;
				if (query_xtL > d1)
				{
					// 18: AL3.push((x, t, d1(x, t)))
					al3->push_back(affected_label(x, t, d1));
				}
				// 19: else
				else
				{
					{
						// 19: PPR[x, hc].push(t)
						mtx_5952[x].lock();
						PPR_insert(*PPR, x, h_c, t);
						mtx_5952[x].unlock();
					}
					{
						// 19: PPR[t, hc].push(x)
						mtx_5952[t].lock();
						PPR_insert(*PPR, t, h_c, x);
						mtx_5952[t].unlock();
					}
				}
			}

			// 20: if r(x) > r(t) then
			if (x < t)
			{
				// 21: d1(t, x) = min({L(tn)[x] + w(t, tn)}), tn ∈ N(t)
				weightTYPE d1 = MAX_VALUE;
				for (int i = 0; i < instance_graph[t].size(); ++i)
				{
					int tn = instance_graph[t][i].first;
					weightTYPE w_tn_t = instance_graph[t][i].second;
					weightTYPE search_result = search_sorted_two_hop_label((*L)[tn], x);
					weightTYPE d1_new = search_result + w_tn_t;
					// update d1
					d1 = min(d1, d1_new);
				}

				// 22: if Query(t, x, L) > d1(t, x) then
				auto query_result =
					graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, t, x);
				weightTYPE query_txL = query_result.first;
				int h_c = query_result.second;
				if (query_txL > d1)
				{
					// 22: AL3.push((t, x, d1(t, x)))
					al3->push_back(affected_label(t, x, d1));
				}
				// 23: else
				else
				{
					{
						// 19: PPR[t, hc].push(x)
						mtx_5952[t].lock();
						PPR_insert(*PPR, t, h_c, x);
						mtx_5952[t].unlock();
					}
					{
						// 19: PPR[x, hc].push(t)
						mtx_5952[x].lock();
						PPR_insert(*PPR, x, h_c, t);
						mtx_5952[x].unlock();
					}
				}
			}
		}
	}
}

void SPREAD3(
		graph_v_of_v_idealID &instance_graph,
		vector<vector<two_hop_label_v1>> *L, 
		PPR_type *PPR,
		std::vector<affected_label> &al3,
		ThreadPool &pool_dynamic, 
		std::vector<std::future<int>> &results_dynamic) {
	/* TODO 4 */
	// 24: for each (u, v, du) ∈ AL3 do
	for (const auto &al3_elem : al3)
	{
		int u = al3_elem.first;
		int v = al3_elem.second;
		weightTYPE du = al3_elem.dis;

		auto query_result =
			graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, u, v);

		// 25: if Q(u, v, L) ≤ du
		if (query_result.first <= du)
		{
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
			continue;
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
			std::pair<int, weightTYPE> topElem = Q.top();
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
				if (v < xn)
				{
					int h_c = 0;
					// 30: if Dis[xn] == -1 then Dis[xn] = Query(xn, v, L)
					if (Dis[xn] == -1)
					{
						auto query_result =
							graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(*L, xn, v);
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
	}
}

void WeightIncreaseMaintenance_improv(graph_v_of_v_idealID &instance_graph, graph_hash_of_mixed_weighted_two_hop_case_info_v1 &mm, int v1, int v2, weightTYPE w_old, weightTYPE w_new,
									  ThreadPool &pool_dynamic, std::vector<std::future<int>> &results_dynamic)
{

	std::vector<affected_label> al1, al3;
	std::vector<pair_label> al2;

	/*it's slow to paralize the following part*/
	for (auto it : mm.L[v1])
	{ 
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
