#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "network.h"

using namespace std;

int network::input_net(const string filename) {
	destruct();

	ifstream inFile(filename.c_str());

	if (!inFile) {
		cerr << "ERROR: No file with name " << filename << ". Stopping initialization..." << endl;
		return -1;
	}

	while (inFile) {
		check_input(inFile);
	}

	inFile.close();

	return 0;
}

void network::check_input(ifstream &inFile) {
	string input;
	getline(inFile, input);

	if (input[0] == '#') {
		if (input.substr(1, 9) == "NET_SIZE:") {
			neurons = atoi(input.substr(10).c_str());
		} else if (input.substr(1, 10) == "MAX_TURNS:") {
			max_turns = atoi(input.substr(11).c_str());
		} else if (input.substr(1, 9) == "RANDOMIZE") {
			out_degree_random();
		} else if (input.substr(1, 7) == "LAMBDA:") {
			double lambda = atof(input.substr(8).c_str());
			out_degree_scale_free(lambda);
		} else if (input.substr(1, 11) == "OUT_DEGREE:") {
			int out_degree = atoi(input.substr(12).c_str());
			out_degree_uniform(out_degree);
		} else if (input.substr(1, 15) == "FIRE_THRESHOLD:") {
			fire_threshold = atof(input.substr(16).c_str());
		} else if (input.substr(1, 11) == "INHIBITION:") {
			inhibition = atof(input.substr(12).c_str());
		} else if (input.substr(1, 21) == "TRANSITION_THRESHOLD:") {
			transition = atof(input.substr(22).c_str());
		} else if (input.substr(1, 6) == "NOISE:") {
			noise_strength = atof(input.substr(7).c_str());
		}
	}
}