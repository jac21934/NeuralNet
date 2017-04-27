#ifndef CONNECTOME_H
#define CONNECTOME_H

#include <random>
#include <cmath>

#include "network_param.h"

void null_connectome(double **weights, int size) {
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			weights[i][j] = 0;
}

template <class distribution, class URNG>
void random_connectome(double **weights, int *character, int size, double inhibitory_fraction, distribution out_dist, URNG &g) {
	null_connectome(weights, size);

	std::bernoulli_distribution inhibit(inhibitory_fraction);
	std::uniform_real_distribution<double> weight_dist(MIN_RES, 1.0);
	std::uniform_int_distribution<int> neuron_dist(0, size - 1);

	for (int i = 0; i < size; i++) {
		character[i] = inhibit(g) ? -1 : 1;

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

#endif /*CONNECTOME_H*/