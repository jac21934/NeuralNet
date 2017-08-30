#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cstring>

#include "network.h"
#include "output.h"

using namespace std;

void network::run(bool verbose) {
	if (!initialized)
		throw runtime_error("Network not initialized");

	double *neuron_last = new double[neurons];
	bool *refractory_last = new bool[neurons];
	double *delta = new double[neurons];

	int last_avalanche = 0;

	for (int t = 0; t < max_turns; t++) {
		memset(delta, 0, neurons * sizeof(double));

		// Avalanche
		int firing_next;
		bool avalanche = false;
		do {
			memcpy(neuron_last, neuron, neurons * sizeof(double));
			memcpy(refractory_last, refractory, neurons * sizeof(bool));

			firing_next = 0;
			
			for (int j = 0; j < neurons; j++) {
				if (refractory_last[j]) {
					refractory[j] = false;
				} else if (neuron_last[j] > fire_threshold) {
					avalanche = true;
					refractory[j] = true;
					neuron[j] = 0;
				} else if (in_degree[j] != 0) {
					for (int i = 0; i < neurons; i++) {
						if (abs(weight[i][j]) > MIN_RES && !refractory_last[i] && neuron_last[i] > fire_threshold) {
							// TODO In principle, there could be a constant coefficient of some kind here. Should it be 1?
							neuron[j] += neuron_last[i] * out_degree[i] * weight[i][j] / in_degree[j];
						}
					}

					delta[j] += neuron[j] - neuron_last[j];
					if (neuron[j] > fire_threshold) {
						firing_next++;
					}
				}
			}
		} while (firing_next > 0);

		if (avalanche) {
			// Reporting
			if (last_avalanche != 0)
				cout << t - last_avalanche << endl;
			last_avalanche = t;

			// Plasticity
			double delta_sum = 0;
			double average_weight_increase = 0;
			for (int i = 0; i < neurons; i++) {
				delta_sum += delta[i];

				// Strengthen active links with positive correlation, weaken negative
				if (delta[i] > 0) {
					for (int j = 0; j < neurons; j++) {
						average_weight_increase += delta[j] / fire_threshold;
						if (weight[i][j] < 0) {
							weight[i][j] -= delta[j] / fire_threshold;
						} else {
							weight[i][j] += delta[j] / fire_threshold;
						}
					}
				}
			}

			// Weaken inactive links
			for (int i = 0; i < neurons; i++) {
				if (delta[i] < 0) {
					for (int j = 0; j < neurons; j++) {
						if (weight[i][j] < 0) {
							weight[i][j] += average_weight_increase / bond_number;
						} else {
							weight[i][j] -= average_weight_increase / bond_number;
						}
					}
				}
			}

			normalize_and_recount();
		
			// Up/down state transition
			if (delta_sum > transition) {
				// down state
				for (int i = 0; i < neurons; i++) {
					if (delta[i] > 0) {
						neuron[i] -= inhibition * delta[i];
					}
				}
			} else if (delta_sum > 0) {
				// up state
				for (int i = 0; i < neurons; i++) {
					if (delta[i] > 0) {
						neuron[i] = fire_threshold * (1 - delta_sum / transition);
					}
				}
			}
		}
		poke_random_neuron();
	}

	delete[] delta;
	delete[] refractory_last;
	delete[] neuron_last;
}

void network::poke_random_neuron(void) {
	int i = rand.random_int() % neurons;
	neuron[i] += rand.random_real() * 0.2;
}

void network::normalize_and_recount(void) {
	bond_number = 0;

	for (int j = 0; j < neurons; j++) {
		in_degree[j] = 0;
	}

	for (int i = 0; i < neurons; i++) {
		out_degree[i] = 0;
		double weight_sum = 0;
		for (int j = 0; j < neurons; j++) {
			if (abs(weight[i][j]) > MIN_RES) {
				out_degree[i]++;
				in_degree[j]++;
				bond_number++;
				weight_sum += abs(weight[i][j]);
			}
		}
		for (int j = 0; j < neurons; j++) {
			weight[i][j] /= weight_sum;
		}
	}
}