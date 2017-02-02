#include <iostream>
#include <chrono>
#include <string>

#include "network.h"
#include "network_param.h"

using namespace std;

int main(int argc, char **argv) {
	string filename = "input.net";
	if (argc == 2) {
		filename = argv[1];
	} else if (argc > 2) {
		cerr << "Usage: " << argv[0] << " [input]" << endl;
		return -1;
	}

	RNG rand(chrono::high_resolution_clock::now().time_since_epoch().count());

	NetworkParams params(filename, rand);

	Network net(params);

	net.run();

	return 0;
}