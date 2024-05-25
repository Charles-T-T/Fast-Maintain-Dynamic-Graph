#pragma once
#include <text_mining/parse_string.h> 
#include <graph_hash_of_mixed_weighted/graph_hash_of_mixed_weighted.h>
#include <string>
#include <vector>
#include <cstring>
#include <fstream>

void graph_hash_of_mixed_weighted_read_graph_with_weight(std::string instance_name,
	graph_hash_of_mixed_weighted& input_graph, double& weight) {

	input_graph.clear();

	std::string line_content;
	std::ifstream myfile(instance_name); // open the file
	if (myfile.is_open()) // if the file is opened successfully
	{
		while (getline(myfile, line_content)) // read file line by line
		{
			std::vector<std::string> Parsed_content = parse_string(line_content, " ");

			if (!Parsed_content[0].compare("Vertex"))
				// when it's equal, compare returns 0
			{
				int v = std::stoi(Parsed_content[1]);
				double nw = std::stod(Parsed_content[2]);
				graph_hash_of_mixed_weighted_add_vertex(input_graph, v, nw);
			}
			else if (!Parsed_content[0].compare("Edge"))
			{
				int v1 = std::stoi(Parsed_content[1]);
				int v2 = std::stoi(Parsed_content[2]);
				double ec = std::stod(Parsed_content[3]);
				graph_hash_of_mixed_weighted_add_edge(input_graph, v1, v2, ec);
			}
			else if (!Parsed_content[0].compare("weight"))
			{
				weight = std::stod(Parsed_content[1]);
			}
		}

		myfile.close(); //close the file
	}
	else
	{
		std::cout << "Unable to open file " << instance_name << std::endl
			<< "Please check the file location or file name." << std::endl; // throw an error message
		getchar(); // keep the console window
		exit(1); // end the program
	}
}