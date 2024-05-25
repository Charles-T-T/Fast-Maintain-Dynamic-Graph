#include <iostream>
#include <chrono>
#include <fstream>
using namespace std;

// header files in the Boost library: https://www.boost.org/ 
#include <boost/random.hpp>
boost::random::mt19937 boost_random_time_seed{ static_cast<std::uint32_t>(std::time(0)) };

#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <graph_hash_of_mixed_weighted/random_graph/graph_hash_of_mixed_weighted_generate_random_graph.h>

void test() {

	/*parameters*/
	int V = 500, E = 500, precision = 1;
	double ec_min = 1, ec_max = 1;

	graph_hash_of_mixed_weighted g = graph_hash_of_mixed_weighted_generate_random_graph(V, E, 0, 0, ec_min, ec_max, precision, boost_random_time_seed);

	std::ofstream outputFile;
	outputFile.precision(10);
	outputFile.setf(std::ios::fixed);
	outputFile.setf(std::ios::showpoint);
	outputFile.open("random_g.csv");
	outputFile << "vertex1,vertex2" << endl;
	for (auto it = g.hash_of_vectors.begin(); it != g.hash_of_vectors.end(); ++it) {
		int v = it->first;
		std::vector<int> adj_v = g.adj_v(v);
		for (int i = adj_v.size() - 1; i >= 0; i--) {
			if (adj_v[i] > v) {
				outputFile << v << "," << adj_v[i] << endl;
			}
		}
	}
	outputFile.close();
}



int main()
{
	test();
}
