
#pragma once
#include <unordered_map>
#include <boost/heap/fibonacci_heap.hpp> 
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

/*in a DBLP graph with 150k vertices and 500k edges,
the below code based on pairing_heap uses 0.72s;
the below code based on fibonacci_heap uses 0.72s;
while the boost code uses 0.2s;
the slowness is likely due to the frequent use of unordered_map*/

struct node_for_sp {
	int index;
	double priority_value;
}; // define the node in the queue
bool operator<(node_for_sp const& x, node_for_sp const& y) {
	return x.priority_value > y.priority_value; // < is the max-heap; > is the min heap
}
typedef typename boost::heap::fibonacci_heap<node_for_sp>::handle_type handle_t_for_sp;

void graph_hash_of_mixed_weighted_shortest_paths_source_to_all(graph_hash_of_mixed_weighted& input_graph, int source,
	std::unordered_map<int, double>& distances, std::unordered_map<int, int>& predecessors) {

	/*Dijkstra¡¯s shortest path algorithm: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
	time complexity: O(|E|+|V|log|V|);
	the output distances and predecessors only contain vertices connected to source*/

	distances.clear();
	predecessors.clear();

	double inf = std::numeric_limits<double>::max();

	node_for_sp node;
	boost::heap::fibonacci_heap<node_for_sp> Q;
	std::unordered_map<int, double> Q_keys;
	std::unordered_map<int, handle_t_for_sp> Q_handles;

	/*time complexity: O(|V|)*/
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		int v = it->first;
		node.index = v;
		node.priority_value = inf;
		Q_keys[v] = inf;
		Q_handles[v] = Q.push(node);
	}

	/*initialize the source*/
	Q_keys[source] = 0;
	node.index = source;
	node.priority_value = Q_keys[source];
	Q.update(Q_handles[source], node);
	//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
	Q.pop();
	distances[source] = 0; // vertices in distances are touched
	predecessors[source] = source;

	auto search = input_graph.hash_of_hashs.find(source);
	if (search != input_graph.hash_of_hashs.end()) {
		for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
			int adj_v = it2->first;
			double ec = it2->second;
			Q_keys[adj_v] = ec;
			node.index = adj_v;
			node.priority_value = Q_keys[adj_v];
			Q.update(Q_handles[adj_v], node);
			predecessors[adj_v] = source;
			//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
		}
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(source);
		for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
			int adj_v = it2->first;
			double ec = it2->second;
			Q_keys[adj_v] = ec;
			node.index = adj_v;
			node.priority_value = Q_keys[adj_v];
			Q.update(Q_handles[adj_v], node);
			predecessors[adj_v] = source;
			//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
		}
	}

	/*time complexity: O(|E|+|V|log|V|) based on fibonacci_heap, not on pairing_heap, which is
	O((|E|+|V|)log|V|);
	there are at most |V| elements in Q; Q only contains vertices not touched*/
	while (Q.size() > 0) {

		node = Q.top();
		Q.pop();
		int top_v = node.index;
		double top_key = Q_keys[top_v];

		distances[top_v] = top_key; // top_v is touched

		auto search = input_graph.hash_of_hashs.find(top_v);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int adj_v = it2->first;
				double ec = it2->second;
				if (distances.count(adj_v) == 0) {
					if (Q_keys[adj_v] > distances[top_v] + ec) { // needs to update key
						Q_keys[adj_v] = distances[top_v] + ec;
						node.index = adj_v;
						node.priority_value = Q_keys[adj_v];
						Q.update(Q_handles[adj_v], node);
						predecessors[adj_v] = top_v;
						//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
					}
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(top_v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int adj_v = it2->first;
				double ec = it2->second;
				if (distances.count(adj_v) == 0) {
					if (Q_keys[adj_v] > distances[top_v] + ec) { // needs to update key
						Q_keys[adj_v] = distances[top_v] + ec;
						node.index = adj_v;
						node.priority_value = Q_keys[adj_v];
						Q.update(Q_handles[adj_v], node);
						predecessors[adj_v] = top_v;
						//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
					}
				}
			}
		}
	}
}




void graph_hash_of_mixed_weighted_shortest_paths_source_to_some(graph_hash_of_mixed_weighted& input_graph, int source,
	std::unordered_set<int>& terminals,
	std::unordered_map<int, double>& distances, std::unordered_map<int, int>& predecessors) {

	/*Dijkstra¡¯s shortest path algorithm: https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
	time complexity: O(|E|+|V|log|V|);
	the output distances and predecessors only contain vertices connected to source*/

	distances.clear();
	predecessors.clear();

	int untouched_terminals_num = terminals.size();
	double inf = std::numeric_limits<double>::max();

	node_for_sp node;
	boost::heap::fibonacci_heap<node_for_sp> Q;
	std::unordered_map<int, double> Q_keys;
	std::unordered_map<int, handle_t_for_sp> Q_handles;

	/*time complexity: O(|V|)*/
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		int v = it->first;
		node.index = v;
		node.priority_value = inf;
		Q_keys[v] = inf;
		Q_handles[v] = Q.push(node);
	}

	/*initialize the source*/
	Q_keys[source] = 0;
	node.index = source;
	node.priority_value = Q_keys[source];
	Q.update(Q_handles[source], node);
	//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
	Q.pop();
	distances[source] = 0; // vertices in distances are touched
	predecessors[source] = source;
	if (terminals.count(source) > 0) {
		untouched_terminals_num--;
		if (untouched_terminals_num == 0) {
			return;
		}
	}

	auto search = input_graph.hash_of_hashs.find(source);
	if (search != input_graph.hash_of_hashs.end()) {
		for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
			int adj_v = it2->first;
			double ec = it2->second;
			Q_keys[adj_v] = ec;
			node.index = adj_v;
			node.priority_value = Q_keys[adj_v];
			Q.update(Q_handles[adj_v], node);
			predecessors[adj_v] = source;
			//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
		}
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(source);
		for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
			int adj_v = it2->first;
			double ec = it2->second;
			Q_keys[adj_v] = ec;
			node.index = adj_v;
			node.priority_value = Q_keys[adj_v];
			Q.update(Q_handles[adj_v], node);
			predecessors[adj_v] = source;
			//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
		}
	}

	/*time complexity: O(|E|+|V|log|V|) based on fibonacci_heap, not on pairing_heap, which is
	O((|E|+|V|)log|V|);
	there are at most |V| elements in Q; Q only contains vertices not touched*/
	while (Q.size() > 0) {

		node = Q.top();
		Q.pop();
		int top_v = node.index;
		double top_key = Q_keys[top_v];

		distances[top_v] = top_key; // top_v is touched
		if (terminals.count(top_v) > 0) {
			untouched_terminals_num--;
			if (untouched_terminals_num == 0) {
				return;
			}
		}

		auto search = input_graph.hash_of_hashs.find(top_v);
		if (search != input_graph.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int adj_v = it2->first;
				double ec = it2->second;
				if (distances.count(adj_v) == 0) {
					if (Q_keys[adj_v] > distances[top_v] + ec) { // needs to update key
						Q_keys[adj_v] = distances[top_v] + ec;
						node.index = adj_v;
						node.priority_value = Q_keys[adj_v];
						Q.update(Q_handles[adj_v], node);
						predecessors[adj_v] = top_v;
						//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
					}
				}
			}
		}
		else {
			auto search2 = input_graph.hash_of_vectors.find(top_v);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int adj_v = it2->first;
				double ec = it2->second;
				if (distances.count(adj_v) == 0) {
					if (Q_keys[adj_v] > distances[top_v] + ec) { // needs to update key
						Q_keys[adj_v] = distances[top_v] + ec;
						node.index = adj_v;
						node.priority_value = Q_keys[adj_v];
						Q.update(Q_handles[adj_v], node);
						predecessors[adj_v] = top_v;
						//std::cout << "Q.update " << node.index << " key:" << node.priority_value << '\n';
					}
				}
			}
		}
	}

}






