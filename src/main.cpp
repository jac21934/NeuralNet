#include <cstdlib>
#include <ctime>
#include <iostream>

#include "network.h"

using namespace std;

int main() {
	srand(time(NULL));

	network net;
	if (net.input_net("input.net") < 0) {
		return -1;
	}

#ifdef DEBUG
	net.run(true);
#else
	net.run(false);
#endif

	return 0;
}