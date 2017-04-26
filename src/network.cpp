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

	int last_avalanche = 0;
	int avalanches = 0;
	bool is_up = false;
	double depol_sum = 0;

	for (int t = 0; t < max_turns; t++) {
		int duration = 0;

		memset(depol, 0, neurons * sizeof(double));
		memset(active, false, neurons * sizeof(bool));

		// Avalanche
		bool keep_going;
		bool avalanche = false;
		do {
			duration++;

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
						if (abs(weight[i][j]) > MIN_RES && !refractory_last[j]) {
							double delta = neuron_last[i] * out_degree[i] * weight[i][j] / in_degree[j] / weight[i][neurons];

							neuron[j] += delta;
							if (!keep_going && neuron[j] > fire_threshold)
								keep_going = true;

							depol[j] += abs(delta);

							if (!isfinite(delta) || !isfinite(depol[j])) {
								cerr << "Something's gone wrong!" << endl;

								cerr << "neuron_last[" << i << "]: " << neuron_last[i] << endl;
								cerr << "refractory_last[" << i << "]: " << refractory_last[i] << endl;
								cerr << "neuron[" << i << "]: " << neuron[i] << endl;
								cerr << "refractory[" << i << "]: " << refractory[i] << endl;

								cerr << "neuron_last[" << j << "]: " << neuron_last[j] << endl;
								cerr << "refractory_last[" << j << "]: " << refractory_last[j] << endl;
								cerr << "neuron[" << j << "]: " << neuron[j] << endl;
								cerr << "refractory[" << j << "]: " << refractory[j] << endl;

								cerr << "out_degree[" << i << "]: " << out_degree[i] << endl;
								cerr << "in_degree[" << j << "]: " << in_degree[j] << endl;
								cerr << "delta: " << delta << endl;
								cerr << "weight[" << i << "][" << j << "]: " << weight[i][j] << endl;
								cerr << "weight sum: " << weight[i][neurons] << endl;
								cerr << "depol[" << j << "]: " << depol[j] << endl;
								exit(-1);
							}
						}
					}
				}
			}
		} while (keep_going);

		if (avalanche) {
			depol_sum = 0;
			double net_weight_increase = 0;
			for (int i = 0; i < neurons; i++) {
				depol_sum += depol[i];

				for (int j = 0; j < neurons; j++) {
					if (weight[i][j] < 0)
						weight[i][j] -= depol[j] / fire_threshold;
					else
						weight[i][j] += depol[j] / fire_threshold;
					net_weight_increase += depol[j] / fire_threshold;
				}
			}

			normalize_and_recount();

			for (int i = 0; i < neurons; i++) {
				for (int j = 0; j < neurons; j++) {
					double delta = min(net_weight_increase / bond_number, abs(weight[i][j]));
					if (weight[i][j] < 0)
						weight[i][j] += delta;
					else
						weight[i][j] -= delta;
				}
			}

			if (wnoise)
				wnoise(weight, neurons);

			normalize_and_recount();

			// Up/down state transition
			if (depol_sum > transition) {
				// down state
				is_up = false;
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] -= disfacilitation * depol[i];
					}
				}
			} else {
				// up state
				is_up = true;
				for (int i = 0; i < neurons; i++) {
					if (active[i]) {
						neuron[i] = fire_threshold * (1 - depol_sum / transition);
					}
				}
			}

			// Reporting
			double weight_sum = 0;
			double num_active = 0;
			for (int i = 0; i < neurons; i++) {
				weight_sum += weight[i][neurons];
				if (active[i])
					num_active++;
			}
			cout << (double) bond_number / neurons / neurons << '\t'
				<< weight_sum << '\t'
				<< t - last_avalanche << '\t'
				<< duration << '\t'
				<< depol_sum << '\t'
				<< num_active << '\t'
				<< is_up << endl;
			last_avalanche = t;
			avalanches++;
		}
		nnoise(neuron, neurons, is_up, transition / depol_sum);
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