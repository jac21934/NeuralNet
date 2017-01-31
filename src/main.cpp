#include <iostream>
#include <chrono>

#include "network.h"
#include "network_param.h"

using namespace std;

int main() {
	RNG rand(chrono::high_resolution_clock::now().time_since_epoch().count());

	NetworkParams params("input.net", rand);

	Network net(params);

	net.run();

	return 0;
}