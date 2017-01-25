#include <stdexcept>
#include <cmath>
#include <random>

#include "network.h"
#include "power_law.h"

using namespace std;

network::network(void) : rand((random_device())()) {
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
		weight[i] = new double[neurons + 1]();
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

	uniform_int_distribution<int> neuron_dist(0, neurons - 1);

	for (int i = 0; i < neurons; i++) {
		set_out_degree(i, neuron_dist(rand));
	}
	normalize_and_recount();
}

void network::out_degree_random(int edges) {
	construct();

	uniform_int_distribution<int> neuron_dist(0, neurons - 1);
	uniform_real_distribution<double> weight_dist(MIN_RES, 1.0);

	int remaining = edges;
	while (remaining > 0) {
		int i = neuron_dist(rand);
		int j = neuron_dist(rand);
		if (i != j && abs(weight[i][j]) < MIN_RES) {
			weight[i][j] = weight_dist(rand);
			remaining--;
		}
	}

	normalize_and_recount();
}

void network::out_degree_scale_free(double lambda) {
	construct();

	power_law_distribution<int> dist(1, neurons, lambda);

	for (int i = 0; i < neurons; i++) {
		set_out_degree(i, dist(rand));
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
	uniform_int_distribution<int> neuron_dist(0, neurons - 1);
	uniform_real_distribution<double> weight_dist(MIN_RES, 1.0);

	int connections_left = degree;
	while (connections_left > 0) {
		int j = neuron_dist(rand);
		if (j != i && abs(weight[i][j]) < MIN_RES) {
			weight[i][j] = weight_dist(rand);
			connections_left--;
		}
	}
}

void network::inhibitory_fraction(double fraction) {
	bernoulli_distribution dist(fraction);

	for (int i = 0; i < neurons; i++) {
		if (dist(rand)) {
			for (int j = 0; j < neurons; j++)
				weight[i][j] = -weight[i][j];
		}
	}
}