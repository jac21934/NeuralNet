#include <climits>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>

#include "network.h"

using namespace std;

network::network(void) {
	neurons = 0;
	max_turns = 0;
	initialized = false;
	noise_strength = 0;
}

network::~network(void) {
	destruct();
}

void network::construct(void) {
	if (initialized) {
		throw runtime_error("Cannot re-initialize without destruct");
	}

	if (neurons <= 0) {
		throw domain_error("Cannot build network of size 0");
	}

	neuron = new double[neurons]();
	out_degree = new int[neurons]();
	in_degree = new int[neurons]();
	weight = new double*[neurons];
	refractory = new bool[neurons]();

	for (int i = 0; i < neurons; i++) {
		weight[i] = new double[neurons]();
	}

	initialized = true;
}

void network::destruct(void) {
	if (initialized) {
		for (int i = 0; i < neurons; i++) {
			delete[] weight[i];
		}

		delete[] weight;
		delete[] in_degree;
		delete[] out_degree;
		delete[] neuron;

		initialized = false;
	}
}

// probably shouldn't use this one
void network::out_degree_random(void) {
	construct();

	for (int i = 0; i < neurons; i++) {
		set_out_degree(i, rand.random_int() % neurons);
	}
	normalize_and_recount();
}

void network::out_degree_random(int edges) {
	construct();

	int remaining = edges;
	while (remaining > 0) {
		int i = rand.random_int() % neurons;
		int j = rand.random_int() % neurons;
		if (i != j && abs(weight[i][j]) < MIN_RES) {
			weight[i][j] = rand.random_interval(-1, 1);
			if (abs(weight[i][j]) > MIN_RES)
				remaining--;
		}
	}

	normalize_and_recount();
}

void network::out_degree_scale_free(double lambda) {
	construct();

	double harmonic = 0;
	for (int i = 1; i <= neurons; i++) {
		harmonic += pow(i, -lambda);
	}

	int i = 0;
	for (int k = 1; i < neurons; k++) {
		int count = min((int) round(pow(k, -lambda) * neurons / harmonic), neurons - i);
		if (count == 0) {
			count = neurons - i;
			k = 1;
		}
		for (; count > 0; count--) {
			set_out_degree(i, k);
			i++;
		}
	}

	normalize_and_recount();
}

void network::out_degree_uniform(int degree) {
	construct();

	for (int i = 0; i < neurons; i++) {
		set_out_degree(i, degree);
	}

	normalize_and_recount();
}

void network::set_out_degree(int i, int degree) {
	int connections_left = degree;
	while (connections_left > 0) {
		int j = rand.random_int() % neurons;
		if (j != i && abs(weight[i][j]) < MIN_RES) {
			weight[i][j] = rand.random_interval(-1, 1);
			if (abs(weight[i][j]) > MIN_RES) {
				connections_left--;
			}
		}
	}
}