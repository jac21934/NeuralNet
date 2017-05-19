#ifndef CONNECTOME_H
#define CONNECTOME_H

#include <random>
#include <cmath>

#include "network_param.h"

void null_connectome(double **weights, int *character, bool *is_out, int size) {
	for (int i = 0; i < size; i++) {
		character[i] = 1;
		is_out[i] = false;

		for (int j = 0; j < size; j++) {
			weights[i][j] = 0;
		}
	}
}

template <class distribution, class URNG>
void random_connectome(double **weights, int *character, bool *is_out, int size, double inhibitory_fraction, double output_fraction, distribution out_dist, URNG &g) {
	null_connectome(weights, character, is_out, size);

	std::bernoulli_distribution inhibit(inhibitory_fraction);
	std::bernoulli_distribution output(output_fraction);
	std::uniform_real_distribution<double> weight_dist(MIN_RES, 1.0);
	std::uniform_int_distribution<int> neuron_dist(0, size - 1);

	for (int i = 0; i < size; i++) {
		character[i] = inhibit(g) ? -1 : 1;

		if (!(is_out[i] = output(g))) {
			int remaining = out_dist(g);

			while (remaining > 0) {
				int j = neuron_dist(g);
				if (i != j && weights[i][j] < MIN_RES) {
					weights[i][j] = weight_dist(g);
					remaining--;
				}
			}
		}
	}
}

#endif /*CONNECTOME_H*/