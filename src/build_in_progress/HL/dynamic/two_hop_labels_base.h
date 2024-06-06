#pragma once
#include <vector>
#include <tool_functions/ThreadPool.h>
#include <shared_mutex>
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <build_in_progress/HL/dynamic/PPR.h>
#include <boost/heap/fibonacci_heap.hpp> 

#define weightTYPE double // float causes error due to "if (query_result.first + 1e-3 >= search_result)" in IncresseMaintenance code

/*PLL label format*/
class two_hop_label_v1 {
public:
	// std::vector<PLL_with_non_adj_reduction_sorted_label> should be sorted by vertex
	int vertex; 
	weightTYPE distance;
};

/* global values that should be cleared after using PLL*/
string reach_limit_error_string_MB = "reach limit error MB";
string reach_limit_error_string_time = "reach limit error time";
long long int max_labal_size_595;
long long int labal_size_595;
auto begin_time_595 = std::chrono::high_resolution_clock::now();
double max_run_time_nanoseconds_595;
bool this_parallel_PLL_PSL_is_running_595 = false;
bool PLL_throw_error = false;
graph_v_of_v_idealID ideal_graph_595;
vector<vector<two_hop_label_v1>> L_temp_595;
PPR_type PPR_595;
std::shared_mutex mtx_595_1, mtx_595_2;
int max_N_ID_for_mtx_595 = 1e7;  // this is the max N to run
vector<std::shared_mutex> mtx_595(max_N_ID_for_mtx_595);  // std::mutex has no copy or move constructor, while std::vector::resize() requires that; you cannot resize mtx;    
// moreover, do not change mtx to a pointer and then points to local values, it is very slow!!
queue<int> Qid_595; // IDs of available elements of P T
vector<vector<weightTYPE>> P_dij_595;
vector<vector<weightTYPE>> T_dij_595;

struct PLL_dynamic_node_for_sp {
public:
	int vertex;
	weightTYPE priority_value;
}; // define the node in the queue
bool operator<(PLL_dynamic_node_for_sp const& x, PLL_dynamic_node_for_sp const& y) {
	return x.priority_value > y.priority_value; // < is the max-heap; > is the min heap
}
typedef typename boost::heap::fibonacci_heap<PLL_dynamic_node_for_sp>::handle_type graph_hash_of_mixed_weighted_HL_PLL_v1_handle_t_for_sp;

vector<vector<graph_hash_of_mixed_weighted_HL_PLL_v1_handle_t_for_sp>> Q_handles_dij_595;

void two_hop_clear_global_values() {

	this_parallel_PLL_PSL_is_running_595 = false;
	graph_v_of_v_idealID().swap(ideal_graph_595);
	vector<vector<two_hop_label_v1>>().swap(L_temp_595);
	PPR_type().swap(PPR_595);
	queue<int>().swap(Qid_595);
	vector<vector<weightTYPE>>().swap(P_dij_595);
	vector<vector<weightTYPE>>().swap(T_dij_595);
}

class graph_hash_of_mixed_weighted_two_hop_case_info_v1 {
public:

	/*running time records*/
	double time_initialization = 0;
	double time_generate_labels = 0;

	/*running limits*/
	long long int max_labal_size = 1e12; // 2-hop-label num
	double max_run_time_seconds = 1e12; // s

	/*labels*/
	vector<vector<two_hop_label_v1>> L;
	PPR_type PPR;

	/*clear labels*/
	void clear_labels() {
		vector<vector<two_hop_label_v1>>().swap(L);	
		PPR_type().swap(PPR);
	}

	long long int compute_L_bit_size() {
		long long int size = 0;
		for (auto it = L.begin(); it != L.end(); it++) {
			size = size + (*it).size() * sizeof(two_hop_label_v1); // 12 bit per two_hop_label_v1
		}
		return size;
	}

	long long int compute_PPR_bit_size() {
		long long int size = 0;
		for (int i = 0; i < PPR.size(); i++) {
			for (int j = 0; j < PPR[i].size(); j++) {
				size = size + (PPR[i][j].second.size() + 1) * sizeof(int); // + 1 ��Ӧ PPR[i][j].first
			}
		}
		return size;
	}

	/*printing*/
	// TODO ??9
	void print_L() {
		cout << "print_L:" << endl;
		for (int i = 0; i < L.size(); i++) {
			cout << "L[" << i << "]=";
			for (int j = 0; j < L[i].size(); j++) {
				cout << "{" << L[i][j].vertex << "," << L[i][j].distance << "}";
			}
			cout << endl;
		}
	}
	void print_PPR() {
		cout << "print_PPR:" << endl;
		for (int i = 0; i < PPR.size(); i++) {
			for (int j = 0; j < PPR[i].size(); j++) {
				cout << "PPR(" << i << "," << PPR[i][j].first << "): ";
				for (int k = 0; k < PPR[i][j].second.size(); k++) {
					cout << PPR[i][j].second[k] << " ";
				}
				cout << endl;
			}
		}
	}
};



auto begin_time = std::chrono::high_resolution_clock::now();
double max_run_time_nanosec;
string reach_limit_time_string = "reach limit time in WeightIncrease";

/*common functions*/

bool compare_two_hop_label_small_to_large(two_hop_label_v1& i, two_hop_label_v1& j)
{
	return i.vertex < j.vertex;  // < is from small to big; > is from big to small
}

// TODO PLL??insert
void insert_sorted_two_hop_label(std::vector<two_hop_label_v1>& input_vector, int key, weightTYPE value) {

	int left = 0, right = input_vector.size() - 1;

	two_hop_label_v1 xx;
	xx.vertex = key;
	xx.distance = value;

	while (left <= right) // it will be skept when input_vector.size() == 0
	{
		int mid = left + ((right - left) / 2); // mid is between left and right (may be equal); 
		if (input_vector[mid].vertex == key) {
			input_vector[mid] = xx;
			return;
		}
		else if (input_vector[mid].vertex > key) {
			right = mid - 1; // the elements after right are always either empty, or have larger keys than input key
		}
		else {
			left = mid + 1; // the elements before left are always either empty, or have smaller keys than input key
		}
	}
	//cout << "h 4.3 " << left << " " << key << " " << value << " " << input_vector.size() << " " << &input_vector << endl;
	input_vector.insert(input_vector.begin() + left, xx);
}

weightTYPE search_sorted_two_hop_label(std::vector<two_hop_label_v1>& input_vector, int key) { 
	// TODO ??7

	/*return true if key is in vector; time complexity O(log n)*/

	int left = 0, right = input_vector.size() - 1;

	while (left <= right) {
		int mid = left + ((right - left) / 2); // mid is between left and right (may be equal); 
		if (input_vector[mid].vertex == key) {
			return input_vector[mid].distance;
		}
		else if (input_vector[mid].vertex > key) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}

	return std::numeric_limits<weightTYPE>::max();
}

pair<weightTYPE, int> search_sorted_two_hop_label2(std::vector<two_hop_label_v1>& input_vector, int key) {

	/*return true if key is in vector; time complexity O(log n)*/

	int left = 0, right = input_vector.size() - 1;

	while (left <= right) {
		int mid = left + ((right - left) / 2); // mid is between left and right (may be equal); 
		if (input_vector[mid].vertex == key) {
			return { input_vector[mid].distance, mid };
		}
		else if (input_vector[mid].vertex > key) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}

	return { std::numeric_limits<weightTYPE>::max(), -1 };
}

#define Query(x, y) graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc(L, x, y) // reduction is not used here
#define Query2(x, y) graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(L, x, y) // reduction is not used here
#define MAX_VALUE std::numeric_limits<weightTYPE>::max()

class affected_label {
public:
	int first, second;
	weightTYPE dis;
	affected_label(){}
	affected_label(int _first, int _second, weightTYPE _dis) {
		first = _first;
		second = _second;
		dis = _dis;
	}
};

class pair_label { // pair_label2 is stored in NoP
public:
	int first, second;
	pair_label(int _first, int _second) {
		first = _first;
		second = _second;
	}
	bool operator == (const pair_label other) const {
		return (first == other.first && second == other.second);
	}
	bool operator < (const pair_label other) const { // used to sort/search pair_label2 in set
		if (first != other.first)
			return first < other.first;
		return second < other.second;
	}
};

struct node_for_DIFFUSE {
	int index;
	weightTYPE disx;
	node_for_DIFFUSE() {}
	node_for_DIFFUSE(int _u, weightTYPE _dis) {
		index = _u;
		disx = _dis;
	}
}; // define the node in the queue

bool operator<(node_for_DIFFUSE const& x, node_for_DIFFUSE const& y) {
	return x.disx > y.disx; // < is the max-heap; > is the min heap
}
typedef typename boost::heap::fibonacci_heap<node_for_DIFFUSE>::handle_type handle_t_for_DIFFUSE; // pairing heap has a similar speed with fibonacci_heap here

vector<std::shared_mutex> mtx_5952(max_N_ID_for_mtx_595);
vector<vector<pair<weightTYPE, int>>> Dis;
vector<vector<weightTYPE>> Q_value;
vector<vector<handle_t_for_DIFFUSE>> Q_handles;

void initialize_global_values_dynamic(int N, int thread_num) {
	Dis.resize(thread_num);
	Q_value.resize(thread_num);
	Q_handles.resize(thread_num);
	queue<int>().swap(Qid_595);
	for (int i = 0; i < thread_num; i++) {
		Dis[i].resize(N, {-1, -1});
		Q_value[i].resize(N, MAX_VALUE);
		Q_handles[i].resize(N);
		Qid_595.push(i);
	}
}






/*codes for querying distances*/

double global_query_times = 0;

weightTYPE graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc(vector<vector<two_hop_label_v1>>& L, int source, int terminal) {

	global_query_times++;

	/*return std::numeric_limits<double>::max() is not connected*/

	if (source == terminal) {
		return 0;
	}

	weightTYPE distance = std::numeric_limits<weightTYPE>::max(); // if disconnected, return this large value

	auto vector1_check_pointer = L[source].begin();
	auto vector2_check_pointer = L[terminal].begin();
	auto pointer_L_s_end = L[source].end(), pointer_L_t_end = L[terminal].end();
	while (vector1_check_pointer != pointer_L_s_end && vector2_check_pointer != pointer_L_t_end) {
		if (vector1_check_pointer->vertex == vector2_check_pointer->vertex) {
			weightTYPE dis = vector1_check_pointer->distance + vector2_check_pointer->distance;
			if (distance > dis) {
				distance = dis;
			}
			vector1_check_pointer++;
		}
		else if (vector1_check_pointer->vertex > vector2_check_pointer->vertex) {
			vector2_check_pointer++;
		}
		else {
			vector1_check_pointer++;
		}
	}

	return distance;

}

pair<weightTYPE, int> graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc2(vector<vector<two_hop_label_v1>>& L, int source, int terminal) {

	global_query_times++;

	/*return std::numeric_limits<double>::max() is not connected*/

	if (source == terminal) {
		return { 0, source };
	}

	weightTYPE distance = std::numeric_limits<weightTYPE>::max(); // if disconnected, return this large value
	int common_hub;

	auto vector1_check_pointer = L[source].begin();
	auto vector2_check_pointer = L[terminal].begin();
	auto pointer_L_s_end = L[source].end(), pointer_L_t_end = L[terminal].end();
	while (vector1_check_pointer != pointer_L_s_end && vector2_check_pointer != pointer_L_t_end) {
		if (vector1_check_pointer->vertex == vector2_check_pointer->vertex) {
			weightTYPE dis = vector1_check_pointer->distance + vector2_check_pointer->distance;
			if (distance > dis) {
				distance = dis;
				common_hub = vector1_check_pointer->vertex;
			}
			vector1_check_pointer++;
		}
		else if (vector1_check_pointer->vertex > vector2_check_pointer->vertex) {
			vector2_check_pointer++;
		}
		else {
			vector1_check_pointer++;
		}
	}

	return { distance , common_hub };

}

weightTYPE graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc3(vector<two_hop_label_v1>& L_s, vector<two_hop_label_v1>& L_t) {

	global_query_times++;

	/*return std::numeric_limits<double>::max() is not connected*/

	weightTYPE distance = std::numeric_limits<weightTYPE>::max(); // if disconnected, return this large value

	auto vector1_check_pointer = L_s.begin();
	auto vector2_check_pointer = L_t.begin();
	auto pointer_L_s_end = L_s.end(), pointer_L_t_end = L_t.end();
	while (vector1_check_pointer != pointer_L_s_end && vector2_check_pointer != pointer_L_t_end) {
		if (vector1_check_pointer->vertex == vector2_check_pointer->vertex) {
			weightTYPE dis = vector1_check_pointer->distance + vector2_check_pointer->distance;
			if (distance > dis) {
				distance = dis;
			}
			vector1_check_pointer++;
		}
		else if (vector1_check_pointer->vertex > vector2_check_pointer->vertex) {
			vector2_check_pointer++;
		}
		else {
			vector1_check_pointer++;
		}
	}

	return distance;

}

pair<weightTYPE, int> graph_hash_of_mixed_weighted_two_hop_v1_extract_distance_no_reduc4(vector<two_hop_label_v1>& L_s, vector<two_hop_label_v1>& L_t) {

	global_query_times++;

	/*return std::numeric_limits<double>::max() is not connected*/

	weightTYPE distance = std::numeric_limits<weightTYPE>::max(); // if disconnected, return this large value
	int common_hub;

	auto vector1_check_pointer = L_s.begin();
	auto vector2_check_pointer = L_t.begin();
	auto pointer_L_s_end = L_s.end(), pointer_L_t_end = L_t.end();
	while (vector1_check_pointer != pointer_L_s_end && vector2_check_pointer != pointer_L_t_end) {
		if (vector1_check_pointer->vertex == vector2_check_pointer->vertex) {
			weightTYPE dis = vector1_check_pointer->distance + vector2_check_pointer->distance;
			if (distance > dis) {
				distance = dis;
				common_hub = vector1_check_pointer->vertex;
			}
			vector1_check_pointer++;
		}
		else if (vector1_check_pointer->vertex > vector2_check_pointer->vertex) {
			vector2_check_pointer++;
		}
		else {
			vector1_check_pointer++;
		}
	}

	return { distance , common_hub };
}