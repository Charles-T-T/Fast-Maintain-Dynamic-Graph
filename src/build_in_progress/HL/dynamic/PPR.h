#pragma once

#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted_binary_operations.h>

/*the pairs should be sorted from small to large based on pair.first, for fast querying*/
typedef std::vector<std::vector<std::pair<int, std::vector<int>>>> PPR_type;


int PPR_binary_operations_insert(std::vector<int>& input_vector, int key) {

	int left = 0, right = input_vector.size() - 1;

	while (left <= right) // it will be skept when input_vector.size() == 0
	{
		int mid = left + ((right - left) / 2); // mid is between left and right (may be equal); 
		if (input_vector[mid] == key) {
			return mid;
		}
		else if (input_vector[mid] > key) {
			right = mid - 1; // the elements after right are always either empty, or have larger keys than input key
		}
		else {
			left = mid + 1; // the elements before left are always either empty, or have smaller keys than input key
		}
	}

	/*the following code is used when key is not in vector, i.e., left > right, specifically, left = right + 1;
	the elements before left are always either empty, or have smaller keys than input key;
	the elements after right are always either empty, or have larger keys than input key;
	so, the input key should be insert between right and left at this moment*/
	input_vector.insert(input_vector.begin() + left, key);
	return left;
}


void PPR_insert(PPR_type& PPR, int v1, int v2, int v3) {

	/*add v3 into PPR(v1, v2)*/

	int pos = graph_hash_of_mixed_weighted_binary_operations_search_position(PPR[v1], v2);
	if (pos == -1) {
		std::vector<int> x = { v3 };
		graph_hash_of_mixed_weighted_binary_operations_insert(PPR[v1], v2, x);
	}
	else {
		PPR_binary_operations_insert(PPR[v1][pos].second, v3);
	}

}

std::vector<int> PPR_retrieve(PPR_type& PPR, int v1, int v2) {

	/*retrieve PPR(v1, v2)*/

	int pos = graph_hash_of_mixed_weighted_binary_operations_search_position(PPR[v1], v2);
	if (pos == -1) {
		std::vector<int> x;
		return x;
	}
	else {
		return PPR[v1][pos].second;
	}

}


void PPR_replace(PPR_type& PPR, int v1, int v2, std::vector<int>& loads) {

	/*replace PPR(v1, v2) = loads*/

	int pos = graph_hash_of_mixed_weighted_binary_operations_search_position(PPR[v1], v2);
	if (pos == -1) {
		graph_hash_of_mixed_weighted_binary_operations_insert(PPR[v1], v2, loads);
	}
	else {
		PPR[v1][pos].second = loads;
	}

}


void PPR_erase(PPR_type& PPR, int v1, int v2, int v3) {
	int pos = graph_hash_of_mixed_weighted_binary_operations_search_position(PPR[v1], v2);
	for (auto it = PPR[v1][pos].second.begin(); it != PPR[v1][pos].second.end(); it++) {
		if (*it == v3) {
			PPR[v1][pos].second.erase(it);
			break;
		}
	}
}


void binary_save_PPR(std::string path, const PPR_type& PPR) {
	std::ofstream FILE(path, std::ios::out | std::ofstream::binary);

	// Store size of the 1st vector
	int s1 = PPR.size();
	FILE.write(reinterpret_cast<const char*>(&s1), sizeof(s1));

	for (auto& v : PPR) {
		// Store size of the 2nd vector
		int size = v.size();
		FILE.write(reinterpret_cast<const char*>(&size), sizeof(size));
		if (size != 0) {
			for (auto& pair_value : v) { // store pair_value one by one
				// Store pair_value.first
				int pair_first_value = pair_value.first;
				FILE.write(reinterpret_cast<const char*>(&pair_first_value), sizeof(pair_first_value));
				// Store size of pair_value.second
				int size2 = pair_value.second.size();
				FILE.write(reinterpret_cast<const char*>(&size2), sizeof(size2));
				if (size2 != 0) {
					// Store pair_value.second
					FILE.write(reinterpret_cast<const char*>(&pair_value.second[0]), size2 * sizeof(int));
				}
			}
		}		
	}
	FILE.close();
}


void binary_read_PPR(std::string path, PPR_type& PPR)
{
	PPR_type().swap(PPR);

	std::ifstream FILE(path, std::ios::in | std::ifstream::binary);

	if (!FILE) {
		std::cout << "Unable to open file " << path << std::endl << "Please check the file location or file name." << std::endl; // throw an error message
		exit(1); // end the program
	}

	// read size of the 1st vector
	int size = 0;
	FILE.read(reinterpret_cast<char*>(&size), sizeof(size));
	PPR.resize(size);

	for (int n = 0; n < size; ++n) {
		// read size of the 2nd vector
		int size2 = 0;
		FILE.read(reinterpret_cast<char*>(&size2), sizeof(size2));
		PPR[n].resize(size2);

		if (size2 == 0) {
			continue;
		}

		for (int k = 0; k < size2; ++k) {
			// read pair_value.first
			int pair_first_value = 0;
			FILE.read(reinterpret_cast<char*>(&pair_first_value), sizeof(pair_first_value));
			PPR[n][k].first = pair_first_value;
			// read size of pair_value.second
			int size3 = 0;
			FILE.read(reinterpret_cast<char*>(&size3), sizeof(size3));
			PPR[n][k].second.resize(size3);

			if (size3 == 0) {
				continue;
			}

			int value = 0;
			for (int j = 0; j < size3; ++j) {
				FILE.read(reinterpret_cast<char*>(&value), sizeof(value));
				PPR[n][k].second[j] = value;
			}
			std::vector<int>(PPR[n][k].second).swap(PPR[n][k].second);
		}
		std::vector<std::pair<int, std::vector<int>>>(PPR[n]).swap(PPR[n]);		
	}
	PPR_type(PPR).swap(PPR);
}







