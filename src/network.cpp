#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cstring>
#include <random>

#include "network.h"

using namespace std;

void Network::run() {
	double *neuron_last = new double[neurons];
	bool *refractory_last = new bool[neurons];
	double *depol = new double[neurons];
	bool *active = new bool[neurons];

	double **weight_old = new double*[neurons];
	for (int i = 0; i < neurons; i++) {
		weight_old[i] = new double[neurons + 1];
	}

	int last_avalanche = 0;

	for (int t = 0; t < max_turns; t++) {
		memset(depol, 0, neurons * sizeof(double));
		memset(active, false, neurons * sizeof(bool));

		for (int i = 0; i < neurons; i++) {
			memcpy(weight_old[i], weight[i], (neurons + 1) * sizeof(double));
		}

		// Avalanche
		bool keep_going;
		bool avalanche = false;
		double net_weight_increase = 0;
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

							if (weight[i][j] < 0)
								weight[i][j] -= abs(delta) / fire_threshold;
							else
								weight[i][j] += abs(delta) / fire_threshold;
							net_weight_increase += abs(delta) / fire_threshold;

							// TODO Should this be depol[j] or depol[i]?
							depol[j] += abs(delta);

							if (!isfinite(delta) || !isfinite(weight[i][j]) || !isfinite(net_weight_increase) || !isfinite(depol[j])) {
								cerr << "Something's gone wrong!" << endl;
								cerr << "neuron_last[" << i << "]: " << neuron_last[i] << endl;
								cerr << "out_degree[" << i << "]: " << out_degree[i] << endl;
								cerr << "in_degree[" << j << "]: " << in_degree[j] << endl;
								cerr << "weight_old[" << i << "][" << j << "]: " << weight_old[i][j] << endl;
								cerr << "delta: " << delta << endl;
								cerr << "weight[" << i << "][" << j << "]: " << weight[i][j] << endl;
								cerr << "net_weight_increase: " << net_weight_increase << endl;
								cerr << "depol[" << j << "]: " << depol[j] << endl;
								exit(-1);
							}
						}
					}
				}
			}
		} while (keep_going);

		if (avalanche) {
			double depol_sum = 0;
			for (int i = 0; i < neurons; i++) {
				depol_sum += depol[i];

				for (int j = 0; j < neurons; j++) {
					double delta = min(net_weight_increase / bond_number, abs(weight[i][j]));
					if (weight[i][j] < 0)
						weight[i][j] += delta;
					else
						weight[i][j] -= delta;
				}
			}
		
			normalize_and_recount();

			// Up/down state transition
			if (depol_sum > transition) {
				// down state
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] -= disfacilitation * depol[i];
					}
				}
			} else {
				// up state
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] = fire_threshold * (1 - depol_sum / transition);
					}
				}
			}

			// Reporting
			if (last_avalanche != 0)
				cout << t - last_avalanche << endl;
			last_avalanche = t;
		}
		noise(neuron, neurons);
	}

	delete[] active;
	delete[] depol;
	delete[] refractory_last;
	delete[] neuron_last;
}

void Network::normalize_and_recount(void) {
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