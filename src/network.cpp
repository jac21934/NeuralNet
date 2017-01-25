#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cstring>
#include <random>

#include "network.h"
#include "output.h"

using namespace std;

void network::run(bool verbose) {
	if (!initialized)
		throw runtime_error("Network not initialized");

	double *neuron_last = new double[neurons];
	bool *refractory_last = new bool[neurons];
	double *depol = new double[neurons];
	bool *active = new bool[neurons];

	double **weight_old = new double*[neurons];
	for (int i = 0; i < neurons; i++) {
		weight_old[i] = new double[neurons + 1];
	}

	int last_avalanche = 0;
	//bool up = true;

	for (int t = 0; t < max_turns; t++) {
		for (int i = 0; i < neurons; i++) {
			memcpy(weight_old[i], weight[i], (neurons + 1) * sizeof(double));
		}
		memset(depol, 0, neurons * sizeof(double));
		memset(active, false, neurons * sizeof(bool));

		// Avalanche
		bool keep_going;
		bool avalanche = false;
		double average_weight_increase = 0;
		do {
			memcpy(neuron_last, neuron, neurons * sizeof(double));
			memcpy(refractory_last, refractory, neurons * sizeof(bool));

			keep_going = false;
			
			for (int i = 0; i < neurons; i++) {
				if (refractory_last[i]) {
					refractory[i] = false;
					neuron[i] = 0;
				} else if (neuron_last[i] > fire_threshold) {
					avalanche = true;
					refractory[i] = true;
					active[i] = true;

					for (int j = 0; j < neurons; j++) {
						if (abs(weight_old[i][j]) > MIN_RES && !refractory_last[j]) {
							double delta = neuron_last[i] * out_degree[i] * weight_old[i][j] / in_degree[j] / weight_old[i][neurons];
							neuron[j] += delta;
							if (!keep_going && neuron[j] > fire_threshold)
								keep_going = true;

							average_weight_increase += abs(delta) / fire_threshold;
							if (weight_old[i][j] < 0)
								weight[i][j] -= abs(delta) / fire_threshold;
							else
								weight[i][j] += abs(delta) / fire_threshold;

							if (delta > 0)
								depol[j] += delta;

							if (!isfinite(weight[i][j]) || !isfinite(average_weight_increase) || !isfinite(depol[j])) {
								cerr << "Shit!" << endl;
								cerr << "weight[" << i << "][" << j << "]: " << weight[i][j] << endl;
								cerr << "average_weight_increase: " << average_weight_increase << endl;
								cerr << "depol[" << j << "]: " << depol[j] << endl;
								exit(-1);
							}
						}
					}
				}
			}
		} while (keep_going);

		if (avalanche) {
			// Reporting
			if (last_avalanche != 0)
				cout << t - last_avalanche << endl;
			last_avalanche = t;

			double depol_sum = 0;
			for (int i = 0; i < neurons; i++) {
				depol_sum += depol[i];

				//if (!active[i]) {
				for (int j = 0; j < neurons; j++) {
					double delta = min(average_weight_increase / bond_number, abs(weight[i][j]));
					if (weight[i][j] < 0)
						weight[i][j] += delta;
					else
						weight[i][j] -= delta;
				}
			}

			// Not-actually-Hebbian Plasticity
			/*double depol_sum = 0;
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
							double delta = average_weight_increase / bond_number;
							if (weight[i][j] < 0) {
								weight[i][j] += delta;
							} else {
								weight[i][j] -= delta;
							}
						}
					}
				}
			} */
		
			normalize_and_recount();

			// Up/down state transition
			if (depol_sum > transition) {
				//up = false;
				// down state
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] -= inhibition * depol[i];
					}
				}
			} else {
				//up = true;
				// up state
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] = fire_threshold * (1 - depol_sum / transition);
					}
				}
			}
		}
		poke_random_neuron();
	}

	for (int i = 0; i < neurons; i++)
		delete[] weight_old[i];
	delete[] weight_old;

	delete[] active;
	delete[] depol;
	delete[] refractory_last;
	delete[] neuron_last;
}

void network::poke_random_neuron(void) {
	uniform_int_distribution<int> neuron_dist(0, neurons - 1);
	uniform_real_distribution<double> noise_dist(0, noise_strength);

	int i = neuron_dist(rand);
	neuron[i] += noise_dist(rand);
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
		weight[i][neurons] = weight_sum;
	}
}