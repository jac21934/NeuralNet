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
	double *depol = new double[neurons];
	double *pre_avalanche = new double[neurons];
	bool *active = new bool[neurons];

	int last_avalanche = 0;

	for (int t = 0; t < max_turns; t++) {
		memset(depol, 0, neurons * sizeof(double));
		memcpy(pre_avalanche, neuron, neurons * sizeof(double));
		memset(active, false, neurons * sizeof(bool));

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
							double delta = neuron_last[i] * out_degree[i] * weight[i][j] / in_degree[j];
							neuron[j] += delta;

							if (delta > 0)
								depol[j] += delta;
						}
					}

					if (neuron[j] > fire_threshold) {
						firing_next++;
						active[j] = true;
					}
				}
			}
		} while (firing_next > 0);

		if (avalanche) {
			// Reporting
			if (last_avalanche != 0)
				cout << t - last_avalanche << endl;
			last_avalanche = t;

			// Not-actually-Hebbian Plasticity
			double depol_sum = 0;
			double average_weight_increase = 0;
			for (int i = 0; i < neurons; i++) {
				depol_sum += depol[i];

				// Strengthen active links with positive correlation, weaken negative
				if (active[i]) {
					for (int j = 0; j < neurons; j++) {
						if (abs(weight[i][j]) > MIN_RES) {
							double delta = (neuron[j] - pre_avalanche[j]) / fire_threshold;
							average_weight_increase += delta;
							if (weight[i][j] < 0) {
								weight[i][j] -= delta;
							} else {
								weight[i][j] += delta;
							}
						}
					}
				}
			}

			// Weaken inactive links
			for (int i = 0; i < neurons; i++) {
				if (!active[i]) {
					for (int j = 0; j < neurons; j++) {
						if (abs(weight[i][j]) > MIN_RES) {
							if (weight[i][j] < 0) {
								weight[i][j] += average_weight_increase / bond_number;
							} else {
								weight[i][j] -= average_weight_increase / bond_number;
							}
						}
					}
				}
			}

			normalize_and_recount();
			if (verbose) { cerr << "Bonds: " << bond_number << endl; }
		
			// Up/down state transition
			if (depol_sum > transition) {
				if (verbose) { cerr << "Down: "; }
				// down state
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] -= inhibition * depol[i];
					}
				}
			} else {
				if (verbose) { cerr << "Up voltage " << fire_threshold * (1 - depol_sum / transition) << ": "; }
				// up state
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] = fire_threshold * (1 - depol_sum / transition);
					}
				}
			}
			if (verbose) { cerr << depol_sum << endl; }
		}
		poke_random_neuron();
	}

	delete[] active;
	delete[] pre_avalanche;
	delete[] depol;
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