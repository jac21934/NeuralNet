#include <stdexcept>
#include <cmath>
#include <random>

#include "network.h"
#include "power_law.h"

using namespace std;

Network::Network(NetworkParams &params) {
	neurons = params.size;
	max_turns = params.max_turns;
	fire_threshold = params.fire_threshold;
	transition = params.transition;
	noise = params.noise;

	neuron = new double[neurons];
	out_degree = new int[neurons]();
	in_degree = new int[neurons]();
	refractory = new bool[neurons]();

	weight = new double*[neurons];
	for (int i = 0; i < neurons; i++)
		weight[i] = new double[neurons + 1]();

	params.builder(weight, neurons);

	for (int i = 0; i < neurons; i++) {
		neuron[i] = 0.9 * fire_threshold;
	}

	normalize_and_recount();
}

Network::~Network(void) {
	for (int i = 0; i < neurons; i++)
		delete[] weight[i];
	delete[] weight;

	delete[] refractory;
	delete[] in_degree;
	delete[] out_degree;
	delete[] neuron;
}