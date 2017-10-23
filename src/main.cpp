#include <iostream>
#include <string>
#include <algorithm>

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
	RNG::result_type entropy[RNG::state_size];
	std::generate(std::begin(entropy), std::end(entropy), std::ref(rd));
	std::seed_seq seed(std::begin(entropy), std::end(entropy));

	RNG rand(seed);

	NetworkParams params(filename, rand);

	Network net(params);

	net.run(std::cout);

	return 0;
}
