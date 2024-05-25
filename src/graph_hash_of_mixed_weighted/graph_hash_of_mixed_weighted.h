#pragma once

#include <unordered_map>
#include <algorithm>
#include <climits>
#include <vector>
#include <text_mining/print_items.h>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted_binary_operations.h>

class graph_hash_of_mixed_weighted_vectors {
public:
	double vertex_weight; // weight of this vertex; only in vectors, not in hashs
	std::vector<std::pair<int, double>> adj_vertices; // adjacenct vertices and weights of edges; ordered from small to large

	void clear() {
		std::vector<std::pair<int, double>>().swap(adj_vertices);
	}
};

class graph_hash_of_mixed_weighted {
public:
	std::unordered_map<int, graph_hash_of_mixed_weighted_vectors> hash_of_vectors; // hash of vectors for all vertices
	std::unordered_map<int, std::unordered_map<int, double>> hash_of_hashs; // hash of hashs for some vertices (adjacenct vertices and weights of edges)

	void clear() {
		std::unordered_map<int, graph_hash_of_mixed_weighted_vectors>().swap(hash_of_vectors);
		std::unordered_map<int, std::unordered_map<int, double>>().swap(hash_of_hashs);
	}

	int degree(int v) {

		/*time complexity O(1)*/

		auto search = hash_of_hashs.find(v);
		if (search != hash_of_hashs.end()) { // v is in hash_of_hashs
			return search->second.size();
		}
		else {
			auto search2 = hash_of_vectors.find(v);
			if (search2 != hash_of_vectors.end()) { // vertex is in input_graph, otherwise return 0
				return search2->second.adj_vertices.size();
			}
			return 0;
		}

	}

	std::vector<int> adj_v(int vertex) { // adj_v may not be sorted

		/*this function returns the adjacent vertices of the input vertex;
		time complexity O(degree of vertex), which is O(|V|) in the worst case*/

		std::vector<int> adj_list;

		auto search = hash_of_hashs.find(vertex);
		if (search != hash_of_hashs.end()) { // vertex is in hash_of_hashs
			for (auto it = search->second.begin(); it != search->second.end(); ++it) {
				adj_list.push_back(it->first);
			}
		}
		else {
			auto search2 = hash_of_vectors.find(vertex);
			if (search2 != hash_of_vectors.end()) { // vertex is in input_graph, otherwise return empty list
				for (auto it = search2->second.adj_vertices.begin(); it != search2->second.adj_vertices.end(); ++it) {
					adj_list.push_back(it->first);
				}
			}
		}
		return adj_list;
	}


	std::vector<std::pair<int, double>> adj_v_and_ec(int vertex) { // sorted list

		/*this function returns the sorted adjacent vertices of the input vertex;
		time complexity O(degree of vertex), which is O(|V|) in the worst case*/

		std::vector<std::pair<int, double>> adj_list;

		auto search = hash_of_hashs.find(vertex);
		if (search != hash_of_hashs.end()) { // vertex is in hash_of_hashs
			for (auto it = search->second.begin(); it != search->second.end(); ++it) {
				graph_hash_of_mixed_weighted_binary_operations_insert(adj_list, it->first, it->second);
			}
		}
		else {
			auto search2 = hash_of_vectors.find(vertex);
			if (search2 != hash_of_vectors.end()) { // vertex is in input_graph, otherwise return empty list
				return search2->second.adj_vertices;
			}
		}
		return adj_list;
	}


	std::pair<int, double> min_adj(int vertex) {
		/*return adj edge with min edge cost*/

		std::pair<int, double> tmp_min = {0, std::numeric_limits<double>::max() };

		auto search = hash_of_hashs.find(vertex);
		if (search != hash_of_hashs.end()) { // vertex is in hash_of_hashs
			for (auto it = search->second.begin(); it != search->second.end(); ++it) {
				if (it->second < tmp_min.second) {
					tmp_min.first = it->first;
					tmp_min.second = it->second;
				}
			}
		}
		else {
			auto search2 = hash_of_vectors.find(vertex);
			if (search2 != hash_of_vectors.end()) { // vertex is in input_graph, otherwise return empty list
				auto adj_list = search2->second.adj_vertices;
				for (auto it = adj_list.begin(); it != adj_list.end(); it++) {
					if (it->second < tmp_min.second) {
						tmp_min.first = it->first;
						tmp_min.second = it->second;
					}
				}
			}
		}
		return tmp_min;
	}


};


/*initially, vertices and adj_vertices are recorded in hash_of_vectors;

When adj_vertices.size() of a vertex >= graph_hash_of_mixed_weighted_turn_on_value, 
record this vertex and its adj_vertices (not vertex weight) in hash_of_hashs,
and clear its adj_vertices in hash_of_vectors;

Later, when adj_vertices.size() of this vertex == graph_hash_of_mixed_weighted_turn_off_value, 
record its adj_vertices in hash_of_vectors again, 
and remove this vertex and its adj_vertices from hash_of_hashs;

graph_hash_of_mixed_weighted_turn_on_value < graph_hash_of_mixed_weighted_turn_off_value, otherwise
vertices may be continuously turning on and off;

small values of graph_hash_of_mixed_weighted_turn_on_value uses more hashs and thus more
memory consumption, but have smaller theoretical time complexities

you can change turn on and off values in your own code to find the faster speed,
 or to balance speed and memory consumption, but please not in this header file

 it is recommended that graph_hash_of_mixed_weighted_turn_on_value >= 2 * graph_hash_of_mixed_weighted_turn_off_value;
 this is like growing and shrinking the hash table; the armotized costs for maintaining this turn on turn off operation is negalatable in a binary way,
 see "Lecture 9: Table Doubling, Karp-Rabin" in "Introduction to Algorithms"
*/
int graph_hash_of_mixed_weighted_turn_on_value = 1e2; // default values
int graph_hash_of_mixed_weighted_turn_off_value = 10;



/*these codes will be widely used:

auto search = g.hash_of_hashs.find(v);
		if (search != g.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int vertex = it2->first;
				double ec = it2->second; // do not remove edges of v in this for loop, otherwise it2 points to outside
			}
		}
		else {
			auto search2 = g.hash_of_vectors.find(v); // if v is not in g, error is here
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int vertex = it2->first;
				double ec = it2->second;  // do not remove edges of v in this for loop, otherwise it2 points to outside
			}
		}




for (auto it1 = movie_links.hash_of_vectors.begin(); it1 != movie_links.hash_of_vectors.end(); it1++) {
		int i = it1->first;

		auto search = movie_links.hash_of_hashs.find(i);
		if (search != movie_links.hash_of_hashs.end()) {
			for (auto it2 = search->second.begin(); it2 != search->second.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double ec = it2->second;
				}
			}
		}
		else {
			auto search2 = movie_links.hash_of_vectors.find(i);
			for (auto it2 = search2->second.adj_vertices.begin(); it2 != search2->second.adj_vertices.end(); it2++) {
				int j = it2->first;
				if (i < j) {
					double ec = it2->second;
				}
			}
		}
	}


*/



bool graph_hash_of_mixed_weighted_compare(const std::pair<int, double>&i, const std::pair<int, double>&j)
{
	return i.first < j.first;  // < is from small to big; > is from big to small.  sort by the first item of pair<int, double>
} // for graph_hash_of_mixed_weighted_turn_off_hash

void graph_hash_of_mixed_weighted_turn_off_hash(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function turns off a vertex from hash_of_hashs;
	time complexity O(sort_complexity of graph_hash_of_mixed_weighted_turn_off_value),
	which can be considered as O(1)*/

	auto search1 = input_graph.hash_of_vectors.find(vertex); // vertex in hash_of_vectors
	auto search2 = input_graph.hash_of_hashs.find(vertex); // vertex in hash_of_hashs

	for (auto it = search2->second.begin(); it != search2->second.end(); it++) {
		search1->second.adj_vertices.push_back({ it->first,it->second }); // copy pair<int, double>  <adj_v, ec>
	}

	/*sort search1->second.adj_vertices from small to large by vertex ID*/
	std::sort(search1->second.adj_vertices.begin(), search1->second.adj_vertices.end(), graph_hash_of_mixed_weighted_compare);

	input_graph.hash_of_hashs.erase(vertex); // remove vertex from hash_of_hashs
}

void graph_hash_of_mixed_weighted_turn_on_hash(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function turns on a vertex in hash_of_hashs;
	time complexity O(graph_hash_of_mixed_weighted_turn_on_value),
	which can be considered as O(1)*/

	auto search1 = input_graph.hash_of_vectors.find(vertex); // vertex in hash_of_vectors

	input_graph.hash_of_hashs[vertex] = {};
	auto search2 = input_graph.hash_of_hashs.find(vertex); // vertex in hash_of_hashs

	for (auto it = search1->second.adj_vertices.begin(); it != search1->second.adj_vertices.end(); it++) {
		search2->second[it->first] = it->second; // copy pair<int, double>  <adj_v, ec>
	}

	search1->second.adj_vertices.clear(); // clear old adj_vertices of vertex in hash_of_vectors
}






void graph_hash_of_mixed_weighted_add_vertex(graph_hash_of_mixed_weighted& input_graph, int vertex, double weight) {

	/*time complexity O(1);

	since unordered_map containers do not allow for duplicate keys, all the vertices are unique;

	this function can also be used to update vertex weight*/

	input_graph.hash_of_vectors[vertex].vertex_weight = weight; // hash_of_vectors contain all vertices
}


void graph_hash_of_mixed_weighted_remove_vertex(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function removes a vertex and its adjacent edges;
	time complexity O(graph_hash_of_mixed_weighted_turn_on_value * |adj_vertices|) in the worst case,
	which can be considered as O(|adj_vertices|)*/

	//std::cout << "graph_hash_of_mixed_weighted_remove_vertex: " << vertex << std::endl;
	//graph_hash_of_mixed_weighted_print(input_graph);

	auto search4 = input_graph.hash_of_hashs.find(vertex);
	if (search4 != input_graph.hash_of_hashs.end()) { // vertex is in hash_of_hashs
		for (auto it = search4->second.begin(); it != search4->second.end(); it++) {
			int adj_v = it->first;
			auto search3 = input_graph.hash_of_hashs.find(adj_v);
			if (search3 != input_graph.hash_of_hashs.end()) { // adj_v is in hash_of_hashs
				search3->second.erase(vertex); // remove vertex
				if (search3->second.size() <= graph_hash_of_mixed_weighted_turn_off_value) {
					graph_hash_of_mixed_weighted_turn_off_hash(input_graph, adj_v);
				}
			}
			else { // adj_v is only in hash_of_vectors
				auto search2 = input_graph.hash_of_vectors.find(adj_v); // adj_v in hash_of_vectors
				graph_hash_of_mixed_weighted_binary_operations_erase(search2->second.adj_vertices, vertex); // remove vertex
			}
		}
		input_graph.hash_of_vectors.erase(vertex); // remove vertex from hash_of_vectors
		input_graph.hash_of_hashs.erase(vertex); // remove vertex from hash_of_hashs
	}
	else { // vertex is only in hash_of_vectors
		auto search1 = input_graph.hash_of_vectors.find(vertex);
		//std::cout << "search1->second.adj_vertices.size(): " << search1->second.adj_vertices.size() << std::endl;
		if (search1 != input_graph.hash_of_vectors.end()) { // vertex is in input_graph
			for (int i = 0; i < search1->second.adj_vertices.size(); i++) {
				int adj_v = search1->second.adj_vertices[i].first;
				//std::cout << "adj_v: " << adj_v << std::endl;
				auto search3 = input_graph.hash_of_hashs.find(adj_v);
				if (search3 != input_graph.hash_of_hashs.end()) { // adj_v is in hash_of_hashs
					search3->second.erase(vertex); // remove vertex
					if (search3->second.size() <= graph_hash_of_mixed_weighted_turn_off_value) {
						graph_hash_of_mixed_weighted_turn_off_hash(input_graph, adj_v);
					}
				}
				else { // adj_v is only in hash_of_vectors
					auto search2 = input_graph.hash_of_vectors.find(adj_v); // adj_v in hash_of_vectors
					graph_hash_of_mixed_weighted_binary_operations_erase(search2->second.adj_vertices, vertex); // remove vertex
				}
			}
			input_graph.hash_of_vectors.erase(vertex); // remove vertex from hash_of_vectors
		}
	}
}


bool graph_hash_of_mixed_weighted_contain_vertex(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function checks whether a vertex/key is in input_graph or not;
	time complexity O(1)*/

	if (input_graph.hash_of_vectors.count(vertex) > 0) { // hash_of_vectors contains all vertices
		return true;
	}
	else {
		return false;
	}

}


void graph_hash_of_mixed_weighted_add_edge(graph_hash_of_mixed_weighted& input_graph, int e1, int e2, double ec) {

	/*this function adds a weighted edge via binary search,
	and may add e1 and e2 into input_graph if they are new vertices;
	time complexity O(graph_hash_of_mixed_weighted_turn_on_value), which can be considered as O(1);
	this function can update edge weights*/

	/*add e2 to adj of e1*/
	auto search = input_graph.hash_of_vectors.find(e1);
	if (search == input_graph.hash_of_vectors.end()) { // e1 is a new vertex
		graph_hash_of_mixed_weighted_add_vertex(input_graph, e1, 0); // add e1; initial weight is 0
		search = input_graph.hash_of_vectors.find(e1);
	}
	auto xx = input_graph.hash_of_hashs.find(e1);
	if (xx != input_graph.hash_of_hashs.end()) { // e1 is in hash_of_hashs
		xx->second[e2] = ec; // add e2 to adj_hash of e1
	}
	else { // e1 is only in hash_of_vectors
		graph_hash_of_mixed_weighted_binary_operations_insert(search->second.adj_vertices, e2, ec);
		if (search->second.adj_vertices.size() >= graph_hash_of_mixed_weighted_turn_on_value) {
			graph_hash_of_mixed_weighted_turn_on_hash(input_graph, e1); // turn on e1
		}
	}

	/*add e1 to adj of e2*/
	search = input_graph.hash_of_vectors.find(e2);
	if (search == input_graph.hash_of_vectors.end()) { // e2 is a new vertex
		graph_hash_of_mixed_weighted_add_vertex(input_graph, e2, 0); // add e2; initial weight is 0
		search = input_graph.hash_of_vectors.find(e2);
	}
	auto yy = input_graph.hash_of_hashs.find(e2);
	if (yy != input_graph.hash_of_hashs.end()) { // e2 is in hash_of_hashs
		yy->second[e1] = ec; // add e1 to adj_hash of e2
	}
	else { // e2 is only in hash_of_vectors
		graph_hash_of_mixed_weighted_binary_operations_insert(search->second.adj_vertices, e1, ec);
		if (search->second.adj_vertices.size() >= graph_hash_of_mixed_weighted_turn_on_value) {
			graph_hash_of_mixed_weighted_turn_on_hash(input_graph, e2); // turn on e2
		}
	}
}


void graph_hash_of_mixed_weighted_edge_weight_plus_value(graph_hash_of_mixed_weighted& input_graph, int e1, int e2, double value) {

	/*this function plus a value to an edge weight;
	we assume that this edge is in the graph, otherwise
	this function will add e1,e2 into input_graph with default edge weight 0;
	this function can NOT be used as "graph_hash_of_mixed_weighted_add_edge",
	as it cannot change existing edge weights directly;
	time complexity O(graph_hash_of_mixed_weighted_turn_on_value), which is O(1)*/


	/*add e2 to adj of e1*/
	auto search = input_graph.hash_of_vectors.find(e1);
	if (search == input_graph.hash_of_vectors.end()) { // e1 is a new vertex
		graph_hash_of_mixed_weighted_add_vertex(input_graph, e1, 0); // add e1; initial weight is 0
		search = input_graph.hash_of_vectors.find(e1);
	}
	auto xx = input_graph.hash_of_hashs.find(e1);
	if (xx != input_graph.hash_of_hashs.end()) { // e1 is in hash_of_hashs
		auto mm = xx->second.find(e2);
		if (mm != xx->second.end()) { // edge e1,e2 exists
			mm->second = mm->second + value; // add value to edge e1,e2
		}
		else {
			xx->second[e2] = value; // add new edge e1,e2
			if (search->second.adj_vertices.size() >= graph_hash_of_mixed_weighted_turn_on_value) {
				graph_hash_of_mixed_weighted_turn_on_hash(input_graph, e1); // turn on e1
			}
		}
	}
	else { // e1 is only in hash_of_vectors
		int pos = graph_hash_of_mixed_weighted_binary_operations_search_position(search->second.adj_vertices, e2);
		if (pos != -1) { // edge e1,e2 exists
			search->second.adj_vertices[pos].second = search->second.adj_vertices[pos].second + value; // add value to e1,e2
		}
		else {
			graph_hash_of_mixed_weighted_binary_operations_insert(search->second.adj_vertices, e2, value);
			if (search->second.adj_vertices.size() >= graph_hash_of_mixed_weighted_turn_on_value) {
				graph_hash_of_mixed_weighted_turn_on_hash(input_graph, e1); // turn on e1
			}
		}
	}

	/*add e1 to adj of e2*/
	search = input_graph.hash_of_vectors.find(e2);
	if (search == input_graph.hash_of_vectors.end()) { // e2 is a new vertex
		graph_hash_of_mixed_weighted_add_vertex(input_graph, e2, 0); // add e2; initial weight is 0
		search = input_graph.hash_of_vectors.find(e2);
	}
	auto yy = input_graph.hash_of_hashs.find(e2);
	if (yy != input_graph.hash_of_hashs.end()) { // e2 is in hash_of_hashs
		auto mm = yy->second.find(e1);
		if (mm != yy->second.end()) { // edge e2,e1 exists
			mm->second = mm->second + value; // add value to edge e2,e1
		}
		else {
			xx->second[e1] = value; // add new edge e2,e1
			if (search->second.adj_vertices.size() >= graph_hash_of_mixed_weighted_turn_on_value) {
				graph_hash_of_mixed_weighted_turn_on_hash(input_graph, e2); // turn on e2
			}
		}
	}
	else { // e2 is only in hash_of_vectors
		int pos = graph_hash_of_mixed_weighted_binary_operations_search_position(search->second.adj_vertices, e1);
		if (pos != -1) { // edge e1,e2 exists
			search->second.adj_vertices[pos].second = search->second.adj_vertices[pos].second + value; // add value to e1,e2
		}
		else {
			graph_hash_of_mixed_weighted_binary_operations_insert(search->second.adj_vertices, e1, value);
			if (search->second.adj_vertices.size() >= graph_hash_of_mixed_weighted_turn_on_value) {
				graph_hash_of_mixed_weighted_turn_on_hash(input_graph, e2); // turn on e2
			}
		}
	}

}


double graph_hash_of_mixed_weighted_edge_weight(graph_hash_of_mixed_weighted& input_graph, int v1, int v2) {

	/*this function return an edge weight; we assume that this edge is in the graph, otherwise large value is returned;
	time complexity O(graph_hash_of_mixed_weighted_turn_on_value), which is O(1)*/

	auto search1 = input_graph.hash_of_hashs.find(v1);
	if (search1 != input_graph.hash_of_hashs.end()) { // v1 is in hash_of_hashs
		auto search2 = search1->second.find(v2);
		if (search2 != search1->second.end()) {  // v2 is adj to v1
			return search1->second[v2];
		}
		else {
			return std::numeric_limits<double>::max();
		}
	}
	else {
		auto search2 = input_graph.hash_of_hashs.find(v2);
		if (search2 != input_graph.hash_of_hashs.end()) {  // v2 is in hash_of_hashs
			auto search3 = search2->second.find(v1);
			if (search3 != search2->second.end()) {  // v1 is adj to v2
				return search2->second[v1];
			}
			else {
				return std::numeric_limits<double>::max();
			}
		}
		else {
			auto search3 = input_graph.hash_of_vectors.find(v1);
			if (search3 != input_graph.hash_of_vectors.end()) { // v1 is in hash_of_vectors
				return graph_hash_of_mixed_weighted_binary_operations_search_weight(search3->second.adj_vertices, v2);
				//int pos = graph_hash_of_mixed_weighted_binary_operations_search_position(search3->second.adj_vertices, v2);
				//if (pos != -1) {
				//	return search3->second.adj_vertices[pos].second;
				//}
				//else {
				//	return INT_MAX;
				//}
			}
			else {
				return std::numeric_limits<double>::max();
			}
		}
	}
}

double graph_hash_of_mixed_weighted_smallest_adj_edge_weight(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	double smallest_adj_ec = INT_MAX;

	auto search = input_graph.hash_of_hashs.find(vertex);
	if (search != input_graph.hash_of_hashs.end()) { // vertex is in hash_of_hashs
		for (auto it = search->second.begin(); it != search->second.end(); ++it) {
			if (smallest_adj_ec > it->second) {
				smallest_adj_ec = it->second;
			}
		}
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(vertex);
		if (search2 != input_graph.hash_of_vectors.end()) { // vertex is in input_graph, otherwise return empty list
			for (auto it = search2->second.adj_vertices.begin(); it != search2->second.adj_vertices.end(); ++it) {
				if (smallest_adj_ec > it->second) {
					smallest_adj_ec = it->second;
				}
			}
		}
	}
	return smallest_adj_ec;

}

double graph_hash_of_mixed_weighted_total_adj_edge_weight(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	double total_adj_ec = 0;

	auto search = input_graph.hash_of_hashs.find(vertex);
	if (search != input_graph.hash_of_hashs.end()) { // vertex is in hash_of_hashs
		for (auto it = search->second.begin(); it != search->second.end(); ++it) {
			total_adj_ec = total_adj_ec + it->second;
		}
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(vertex);
		if (search2 != input_graph.hash_of_vectors.end()) { // vertex is in input_graph, otherwise return empty list
			for (auto it = search2->second.adj_vertices.begin(); it != search2->second.adj_vertices.end(); ++it) {
				total_adj_ec = total_adj_ec + it->second;
			}
		}
	}
	return total_adj_ec;

}

bool graph_hash_of_mixed_weighted_contain_edge(graph_hash_of_mixed_weighted& input_graph, int v1, int v2) {

	/*this function checks whether an edge is in input_graph or not;
	time complexity O(log graph_hash_of_mixed_weighted_turn_on_value), which is O(1)*/

	if (input_graph.hash_of_vectors.count(v1) == 0 || input_graph.hash_of_vectors.count(v2) == 0) {
		return false;
	}

	auto search1 = input_graph.hash_of_hashs.find(v1);
	if (search1 != input_graph.hash_of_hashs.end()) { // v1 is in hash_of_hashs
		auto search2 = search1->second.find(v2);
		if (search2 != search1->second.end()) {  // v2 is adj to v1
			return true;
		}
		else {
			return false;
		}
	}
	else {
		auto search3 = input_graph.hash_of_vectors.find(v1);
		if (graph_hash_of_mixed_weighted_binary_operations_search(search3->second.adj_vertices, v2)) {
			return true;
		}
		return false;
	}
}

void graph_hash_of_mixed_weighted_remove_edge_and_isolated_vertices(graph_hash_of_mixed_weighted& input_graph, int e1, int e2) {

	/*this function removes an edge, and may remove e1 and e2 if they are isolated;
	time complexity O(graph_hash_of_mixed_weighted_turn_on_value), which is O(1)*/

	auto search = input_graph.hash_of_hashs.find(e1);
	if (search != input_graph.hash_of_hashs.end()) { // e1 is in hash_of_hashs
		search->second.erase(e2); // remove e2 from adj of e1
		if (search->second.size() <= graph_hash_of_mixed_weighted_turn_off_value) {
			graph_hash_of_mixed_weighted_turn_off_hash(input_graph, e1);
		}
	}
	else { // e1 is only in hash_of_vectors
		auto search2 = input_graph.hash_of_vectors.find(e1);
		if (search2 != input_graph.hash_of_vectors.end()) { // e1 is in hash_of_vectors
			graph_hash_of_mixed_weighted_binary_operations_erase(search2->second.adj_vertices, e2);  // remove e2 from adj of e1
		}
	}
	if (input_graph.hash_of_hashs.count(e1) == 0 && input_graph.hash_of_vectors[e1].adj_vertices.size() == 0) { // e1 is isolated
		input_graph.hash_of_vectors.erase(e1); // remove e1
	}

	search = input_graph.hash_of_hashs.find(e2);
	if (search != input_graph.hash_of_hashs.end()) { // e2 is in hash_of_hashs
		search->second.erase(e1); // remove e1 from adj of e2
		if (search->second.size() <= graph_hash_of_mixed_weighted_turn_off_value) {
			graph_hash_of_mixed_weighted_turn_off_hash(input_graph, e2);
		}
	}
	else { // e2 is only in hash_of_vectors
		auto search2 = input_graph.hash_of_vectors.find(e2);
		if (search2 != input_graph.hash_of_vectors.end()) { // e2 is in hash_of_vectors
			graph_hash_of_mixed_weighted_binary_operations_erase(search2->second.adj_vertices, e1);  // remove e1 from adj of e2
		}
	}
	if (input_graph.hash_of_hashs.count(e2) == 0 && input_graph.hash_of_vectors[e2].adj_vertices.size() == 0) { // e2 is isolated
		input_graph.hash_of_vectors.erase(e2); // remove e2
	}
}

void graph_hash_of_mixed_weighted_remove_edge_but_not_isolated_vertices(graph_hash_of_mixed_weighted& input_graph, int e1, int e2) {

	/*this function removes an edge, and does not remove e1 and e2 if they are isolated;
	time complexity O(graph_hash_of_mixed_weighted_turn_on_value), which is O(1)*/

	auto search = input_graph.hash_of_hashs.find(e1);
	if (search != input_graph.hash_of_hashs.end()) { // e1 is in hash_of_hashs
		search->second.erase(e2); // remove e2 from adj of e1
		if (search->second.size() <= graph_hash_of_mixed_weighted_turn_off_value) {
			graph_hash_of_mixed_weighted_turn_off_hash(input_graph, e1);
		}
	}
	else { // e1 is only in hash_of_vectors
		auto search3 = input_graph.hash_of_vectors.find(e1);
		if (search3 != input_graph.hash_of_vectors.end()) { // e1 exist!
			graph_hash_of_mixed_weighted_binary_operations_erase(search3->second.adj_vertices, e2);  // remove e2 from adj of e1
		}
	}

	search = input_graph.hash_of_hashs.find(e2);
	if (search != input_graph.hash_of_hashs.end()) { // e2 is in hash_of_hashs
		search->second.erase(e1); // remove e1 from adj of e2
		if (search->second.size() <= graph_hash_of_mixed_weighted_turn_off_value) {
			graph_hash_of_mixed_weighted_turn_off_hash(input_graph, e2);
		}
	}
	else { // e2 is only in hash_of_vectors
		auto search3 = input_graph.hash_of_vectors.find(e2);
		if (search3 != input_graph.hash_of_vectors.end()) { // e2 exist!
			graph_hash_of_mixed_weighted_binary_operations_erase(search3->second.adj_vertices, e1);  // remove e1 from adj of e2
		}	
	}
}




graph_hash_of_mixed_weighted graph_hash_of_mixed_weighted_copy_graph(graph_hash_of_mixed_weighted& input_graph) {

	return input_graph;

}


int graph_hash_of_mixed_weighted_num_vertices(graph_hash_of_mixed_weighted& input_graph) {

	/*time complexity O(1)*/

	return input_graph.hash_of_vectors.size();

}


long long int graph_hash_of_mixed_weighted_num_edges(graph_hash_of_mixed_weighted& input_graph) {

	/*time complexity O(|V|)*/

	long long int num = 0;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); ++it) {
		int v = it->first;
		auto search = input_graph.hash_of_hashs.find(v);
		if (search != input_graph.hash_of_hashs.end()) { // v is in hash_of_hashs
			num = num + search->second.size();
		}
		else { // v is only in hash_of_vectors
			num = num + it->second.adj_vertices.size();
		}


	}
	return num / 2;
}


double graph_hash_of_mixed_weighted_total_RAM_MB(graph_hash_of_mixed_weighted& g) {

	/*
	we assume that the edge weight type is float; and the structure is vector of vectors (only edge weighted)
	*/

	double bit_num = (double)graph_hash_of_mixed_weighted_num_vertices(g) * sizeof(std::vector <std::pair<int, double>>) + graph_hash_of_mixed_weighted_num_edges(g) * sizeof(std::pair<int, double>); // two pointers for each vector

	return bit_num / 1024 / 1024;
}


std::list<int> graph_hash_of_mixed_weighted_adjacent_vertices(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function returns the adjacent vertices of the input vertex;
	time complexity O(degree of vertex), which is O(|V|) in the worst case*/

	std::list<int> adj_list;

	auto search = input_graph.hash_of_hashs.find(vertex);
	if (search != input_graph.hash_of_hashs.end()) { // vertex is in hash_of_hashs
		for (auto it = search->second.begin(); it != search->second.end(); ++it) {
			adj_list.push_back(it->first);
		}
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(vertex);
		if (search2 != input_graph.hash_of_vectors.end()) { // vertex is in input_graph, otherwise return empty list
			for (auto it = search2->second.adj_vertices.begin(); it != search2->second.adj_vertices.end(); ++it) {
				adj_list.push_back(it->first);
			}
		}	
	}
	return adj_list;
}


std::list<std::pair<int, double>> graph_hash_of_mixed_weighted_adjacent_vertices_and_weights(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function returns the adjacent vertices of the input vertex;
	time complexity O(degree of vertex), which is O(|V|) in the worst case*/

	std::list<std::pair<int, double>> adj_list;

	auto search = input_graph.hash_of_hashs.find(vertex);
	if (search != input_graph.hash_of_hashs.end()) { // vertex is in hash_of_hashs
		for (auto it = search->second.begin(); it != search->second.end(); ++it) {
			adj_list.push_back({ it->first, it->second });
		}
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(vertex);
		if (search2 != input_graph.hash_of_vectors.end()) { // vertex is in input_graph, otherwise return empty list
			for (auto it = search2->second.adj_vertices.begin(); it != search2->second.adj_vertices.end(); ++it) {
				adj_list.push_back({ it->first, it->second });
			}
		}
	}
	return adj_list;
}


int graph_hash_of_mixed_weighted_adjacent_vertices_size(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function returns the size of adjacent vertices of the input vertex;
	time complexity O(1)*/

	auto search = input_graph.hash_of_hashs.find(vertex);
	if (search != input_graph.hash_of_hashs.end()) { // vertex is in hash_of_hashs
		return search->second.size();
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(vertex);
		return search2->second.adj_vertices.size();
	}
}


int graph_hash_of_mixed_weighted_degree(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*time complexity O(1)*/

	auto search = input_graph.hash_of_hashs.find(vertex);
	if (search != input_graph.hash_of_hashs.end()) { // vertex is in hash_of_hashs
		return search->second.size();
	}
	else {
		auto search2 = input_graph.hash_of_vectors.find(vertex);
		if (search2 != input_graph.hash_of_vectors.end()) { // vertex is in input_graph, otherwise return 0
			return search2->second.adj_vertices.size();
		}
		return 0;
	}
}


int graph_hash_of_mixed_weighted_vertex_list_ID(graph_hash_of_mixed_weighted& input_graph, int vertex) {

	/*this function returns the ID of the hash that corresponds to the vertex;
	time complexity O(|V|)*/
	int i = 0;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); it++) {
		if ((*it).first == vertex) {
			return i;
		}
		i++;
	}
	return INT_MAX;
}


void graph_hash_of_mixed_weighted_print (graph_hash_of_mixed_weighted& input_graph) {
	// TODO Œ Ã‚9
	/*this function prints the whole graph_hash_of_mixed_weighted*/

	int print_relex_num = 100;
	int num = 0;
	//std::cout << '\n';
	std::cout << "graph_hash_of_mixed_weighted_print: " << std::endl;
	std::cout << "|V|=" << input_graph.hash_of_vectors.size() <<
		" |E|=" << graph_hash_of_mixed_weighted_num_edges(input_graph) << std::endl;

	std::cout << "hash_of_vectors:" << std::endl;
	for (auto it = input_graph.hash_of_vectors.begin(); it != input_graph.hash_of_vectors.end(); ++it) {
		std::cout << "Vertex: " << it->first << " Weight: " << it->second.vertex_weight << '\n';
		std::cout << "       adj_vertices (max " << graph_hash_of_mixed_weighted_turn_on_value << "): ";
		for (int i = 0; i < it->second.adj_vertices.size(); i++) {
			std::cout << "<" << it->second.adj_vertices[i].first << "," << it->second.adj_vertices[i].second << "> ";
		}
		std::cout << '\n';

		num++;
		if (num%print_relex_num == 0) {
			getchar();
		}
	}

	std::cout << "hash_of_hashs:" << std::endl;
	for (auto it = input_graph.hash_of_hashs.begin(); it != input_graph.hash_of_hashs.end(); ++it) {
		std::cout << "Vertex: " << it->first << '\n';
		std::cout << "       adj_vertices (max " << graph_hash_of_mixed_weighted_turn_on_value << "): ";
		for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			std::cout << "<" << it2->first << "," << it2->second << "> ";
		}
		std::cout << '\n';

		num++;
		if (num%print_relex_num == 0) {
			getchar();
		}
	}

	std::cout << "Print END" << '\n';
	std::cout << '\n';
}


void graph_hash_of_mixed_weighted_print_size(graph_hash_of_mixed_weighted& input_graph) {

	int num = 0;
	//std::cout << '\n';
	std::cout << "graph_hash_of_mixed_weighted_print_size: |V|=" << input_graph.hash_of_vectors.size() <<
		" |E|=" << graph_hash_of_mixed_weighted_num_edges(input_graph) << std::endl;
}











/*the following are test codes

------------------
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>

int main()
{
	graph_hash_of_mixed_weighted_example();
}
---------------------------
*/


void test_graph_hash_of_mixed_weighted() {

	graph_hash_of_mixed_weighted g;

	graph_hash_of_mixed_weighted_add_vertex(g, 1, 0.1);
	graph_hash_of_mixed_weighted_add_vertex(g, 2, 0.1);
	graph_hash_of_mixed_weighted_add_vertex(g, 3, 0.1);
	graph_hash_of_mixed_weighted_add_vertex(g, 4, 0.1);

	graph_hash_of_mixed_weighted_add_edge(g, 1, 12, 0.5);
	graph_hash_of_mixed_weighted_add_edge(g, 1, 2, 0.5);
	graph_hash_of_mixed_weighted_add_edge(g, 4, 2, 0.5);
	graph_hash_of_mixed_weighted_add_edge(g, 12, 1, 0.5);
	graph_hash_of_mixed_weighted_add_edge(g, 1, 12, 0.45);
	graph_hash_of_mixed_weighted_add_edge(g, 1, 4, 0.5);
	graph_hash_of_mixed_weighted_add_edge(g, 1, 3, 0.5);

	std::pair<int, double> min_adj = g.min_adj(1);
	std::cout << "min_adj: " << min_adj.first << ", " << min_adj.second << std::endl;

	graph_hash_of_mixed_weighted_print(g);

	std::list<int> xx = graph_hash_of_mixed_weighted_adjacent_vertices(g, 1);
	print_list_int(xx);

	std::cout << "graph_hash_of_mixed_weighted_contain_vertex(g,2): " << graph_hash_of_mixed_weighted_contain_vertex(g, 2) << std::endl;
	graph_hash_of_mixed_weighted_remove_vertex(g, 2);
	std::cout << "graph_hash_of_mixed_weighted_contain_vertex(g,2): " << graph_hash_of_mixed_weighted_contain_vertex(g, 2) << std::endl;

	std::cout << "graph_hash_of_mixed_weighted_degree(g,1): " << graph_hash_of_mixed_weighted_degree(g, 1) << std::endl;
	std::cout << "graph_hash_of_mixed_weighted_degree(g,2): " << graph_hash_of_mixed_weighted_degree(g, 2) << std::endl;

	std::cout << "graph_hash_of_mixed_weighted_edge_weight(g,1,2): " << graph_hash_of_mixed_weighted_edge_weight(g, 1, 2) << std::endl;
	std::cout << "graph_hash_of_mixed_weighted_edge_weight(g,1,3): " << graph_hash_of_mixed_weighted_edge_weight(g, 1, 3) << std::endl;

	std::cout << "graph_hash_of_mixed_weighted_contain_edge(g,1,2): " << graph_hash_of_mixed_weighted_contain_edge(g, 1, 2) << std::endl;
	std::cout << "graph_hash_of_mixed_weighted_contain_edge(g,1,3): " << graph_hash_of_mixed_weighted_contain_edge(g, 1, 3) << std::endl;


	
	
	graph_hash_of_mixed_weighted_remove_edge_but_not_isolated_vertices(g, 1, 4);
	graph_hash_of_mixed_weighted_remove_edge_but_not_isolated_vertices(g, 1, 3);
	graph_hash_of_mixed_weighted_remove_edge_but_not_isolated_vertices(g, 1, 3);
	graph_hash_of_mixed_weighted_print(g);

	graph_hash_of_mixed_weighted_remove_edge_and_isolated_vertices(g, 1, 4);
	graph_hash_of_mixed_weighted_remove_edge_and_isolated_vertices(g, 1, 3);
	graph_hash_of_mixed_weighted_remove_edge_and_isolated_vertices(g, 1, 4);


	graph_hash_of_mixed_weighted_print(g);

}

void graph_hash_of_mixed_weighted_example() {

	/*
	Create a complete graph of 10 nodes
	Weight of edge (u,v) and (v,u) equal to min(u,v)+max(u,v)*0.1
	Weight of vertex i equal to i*0.1
	*/
	using std::cout;
	int N = 10;
	graph_hash_of_mixed_weighted g;
	//===============================================================
		/*
		Insert the vertex
		When the vertex exists, it will update its weight.
		*/
	for (int i = 0; i < N; i++) {
		graph_hash_of_mixed_weighted_add_vertex(g, i, i * 0.1);
	}
	//===============================================================
		/*
		Insert the edge
		When the edge exists, it will update its weight.
		*/
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			graph_hash_of_mixed_weighted_add_edge(g, i, j, j + 0.1 * i); // Insert the edge(i,j) with value j+0.1*i
		}
	}
	//===============================================================
		/*
		A group of queries
		1.Get the number of vertex.
			The output is 10
		2.Get the number of edges, (u,v) and (v,u) only be counted once.
			The output is 45 (10*9/2)
		3.Get the least weight edge from point 3.
			The output is 0.3, edge is (3,0)
		4.Check if graph contain the edge (3,1).
			The output is True(1)
		5.Get the value of edge(3,1).
			The output is 1.3
		*/
	cout << graph_hash_of_mixed_weighted_num_vertices(g) << " "
		<< graph_hash_of_mixed_weighted_num_edges(g) << " "
		<< graph_hash_of_mixed_weighted_smallest_adj_edge_weight(g, 3) << " "
		<< graph_hash_of_mixed_weighted_contain_edge(g, 3, 1) << " "
		<< graph_hash_of_mixed_weighted_edge_weight(g, 3, 1) << '\n';
	//===============================================================
		/*
		Remove all the edge starting from 0 1
			using different remove function:
			- graph_hash_of_mixed_weighted_remove_edge_and_isolated_vertices: will remove vertic if vertic is isolated
			- graph_hash_of_mixed_weighted_remove_edge_but_not_isolated_vertices: otherwise
			If the edge does not exist, it will do nothing.
		Remove the vertex 2
			It will remove all the edge connecting to it as well
		Get the number of vertex. The output is 8
		Get the number of edges. The output is 21(7*6/2)
		Check if graph contain vertic 0. The output is False(0)
		Get the ID of vertic 5
			The output is 4
			The hash vector is 1 2 3 4 5 6 7 8 9, 5 is NO.3 in the vector(startin from 0)
		*/
	for (int i = 0; i < N; i++) {
		graph_hash_of_mixed_weighted_remove_edge_and_isolated_vertices(g, 0, i);
		graph_hash_of_mixed_weighted_remove_edge_but_not_isolated_vertices(g, 1, i);
	}
	graph_hash_of_mixed_weighted_remove_vertex(g, 2);
	std::cout << graph_hash_of_mixed_weighted_num_vertices(g) << " "
		<< graph_hash_of_mixed_weighted_num_edges(g) << " "
		<< graph_hash_of_mixed_weighted_contain_vertex(g, 0) << " "
		<< graph_hash_of_mixed_weighted_vertex_list_ID(g, 5) << '\n';
	//===============================================================
		/*
		Plus all the edge with value 5.
			If the edge does not exist, it will create an edge with weight v.
			After plusing, it becomes complete graph again.
		Get the vertic adjacent list of 0
			It contains 1~9
		Get the vertic adjacent and weight list of 5
			It contains 0~9 except 5
			The weight of edge is:
				(5,0)(5,1)(5,2): they are newly added, the value is 5
				(5,3)~(5,9): the value is plused by 5
		*/
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			graph_hash_of_mixed_weighted_edge_weight_plus_value(g, i, j, 5);
		}
	}
	std::list<int>  adjacent_list = graph_hash_of_mixed_weighted_adjacent_vertices(g, 0);
	for (auto i : adjacent_list)
		std::cout << i << " ";
	std::cout << '\n';
	std::list<std::pair<int, double>>  adjacent_weight_list = graph_hash_of_mixed_weighted_adjacent_vertices_and_weights(g, 5);
	for (auto i : adjacent_weight_list)
		std::cout << i.first << " " << i.second << " | " << " ";
	std::cout << '\n';
	//===============================================================
		/*
		Print the information of the graph
		*/
	graph_hash_of_mixed_weighted_print(g);
	graph_hash_of_mixed_weighted_print_size(g);
}
