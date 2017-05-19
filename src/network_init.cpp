#include <stdexcept>
#include <cmath>
#include <random>

#include "network.h"
#include "power_law.h"

using namespace std;

Network::Network(NetworkParams &params) {
	neurons = params.size;
	avalanches = params.avalanches;
	fire_threshold = params.fire_threshold;
	transition = params.transition;
	nnoise = params.nnoise;
	wnoise = params.wnoise;

	neuron = new double[neurons];
	character = new int[neurons];
	is_out = new bool[neurons]();
	out_degree = new int[neurons]();
	in_degree = new int[neurons]();
	refractory = new bool[neurons]();

	weight = new double*[neurons];
	for (int i = 0; i < neurons; i++)
		weight[i] = new double[neurons + 1];

	params.builder(weight, character, is_out, neurons);

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
	delete[] is_out;
	delete[] character;
	delete[] neuron;
}