#include <fstream>
#include <string>
#include <functional>
#include <iostream>

#include "network_param.h"
#include "power_law.h"
#include "connectome.h"
#include "noise.h"

using namespace std;

NetworkParams::NetworkParams(const string &filename, RNG &g): g(g) {
	size = 0;
	max_turns = 0;
	fire_threshold = 0;
	disfacilitation = 0;
	transition = 0;
	builder = NULL;
	nnoise = NULL;
	wnoise = NULL;

	ifstream inFile(filename.c_str());

	if (!inFile)
		throw runtime_error("No file with name \"" + filename + "\"");

	while (inFile)
		parse_file(inFile);

	inFile.close();
}

void NetworkParams::parse_file(ifstream &inFile) {
	string input;
	getline(inFile, input);

	if (input[0] == '#') {
		if (input.substr(1, 9) == "NET_SIZE:") {
			size = atoi(input.substr(10).c_str());

		} else if (input.substr(1, 10) == "MAX_TURNS:") {
			max_turns = atoi(input.substr(11).c_str());

		} else if (input.substr(1, 15) == "FIRE_THRESHOLD:") {
			fire_threshold = atof(input.substr(16).c_str());

		} else if (input.substr(1, 16) == "DISFACILITATION:") {
			disfacilitation = atof(input.substr(17).c_str());

		} else if (input.substr(1, 21) == "TRANSITION_THRESHOLD:") {
			transition = atof(input.substr(22).c_str());

		} else if (input.substr(1, 7) == "LAMBDA:") {
			size_t comma = input.find(',');
			double lambda = atof(input.substr(8, comma - 8).c_str());
			double inhibit = atof(input.substr(comma + 1).c_str());

			power_law_distribution<int> dist(min(2, size - 1), min(100, size - 1), lambda);

			builder = bind(random_connectome<power_law_distribution<int>, RNG>, placeholders::_1, placeholders::_2, inhibit, dist, g);

		} else if (input.substr(1, 11) == "OUT_DEGREE:") {
			size_t comma = input.find(',');
			int out_degree = atoi(input.substr(12, comma - 12).c_str());
			double inhibit = atof(input.substr(comma + 1).c_str());

			uniform_int_distribution<int> dist(out_degree, out_degree);

			builder = bind(random_connectome<uniform_int_distribution<int>, RNG>, placeholders::_1, placeholders::_2, inhibit, dist, g);

		} else if (input.substr(1, 13) == "NEURON_NOISE:") {
			size_t comma = input.find(',');
			double mean = atof(input.substr(14, comma - 14).c_str());
			double stdev = atof(input.substr(comma + 1).c_str());

			normal_distribution<double> dist(mean, stdev);

			nnoise = bind(neuron_noise<normal_distribution<double>, RNG>, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, dist, g);
		} else if (input.substr(1, 13) == "WEIGHT_NOISE:") {
			size_t comma = input.find(',');
			double mean = atof(input.substr(14, comma - 14).c_str());
			double stdev = atof(input.substr(comma + 1).c_str());

			normal_distribution<double> dist(mean, stdev);

			wnoise = bind(weight_noise<normal_distribution<double>, RNG>, placeholders::_1, placeholders::_2, dist, g);
		}
	}
}