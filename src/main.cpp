#include <iostream>

#include "network.h"

using namespace std;

int main() {
	network net;
	if (net.input_net("input.net") < 0) {
		return -1;
	}

	net.run();

	return 0;
}