#include <iostream>
#include <chrono>
#include <string>
#include <vector>

#include "network.h"
#include "network_param.h"

int main(int argc, char **argv) {
	std::string filename = "input.net";
	if (argc == 2) {
		filename = argv[1];
	} else if (argc > 2) {
		std::cerr << "Usage: " << argv[0] << " [input]" << std::endl;
		return -1;
	}

	// Build up some entropy for the RNG
	std::random_device rd;
	std::vector<unsigned int> entropy;
	for (int i = 0; i < 624; i++) {
		entropy.push_back(rd());
	}
	std::seed_seq seed(entropy.begin(), entropy.end());
	RNG rand(seed);

	NetworkParams params(filename, rand);

	Network net(params);

	net.run(std::cout);

	return 0;
}
