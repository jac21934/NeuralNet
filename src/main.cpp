#include <iostream>
#include <chrono>
#include <string>

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

	RNG rand(
		std::chrono::high_resolution_clock::now().time_since_epoch().count());

	NetworkParams params(filename, rand);

	Network net(params);

	net.run(std::cout);

	return 0;
}