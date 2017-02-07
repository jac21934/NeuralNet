#ifndef NOISE_H
#define NOISE_H

#include <random>

template<class distribution, class URNG>
void neuron_noise(double *neurons, int size, distribution &noise_dist, URNG &g) {
	std::uniform_int_distribution<int> neuron_dist(0, size - 1);

	neurons[neuron_dist(g)] += noise_dist(g);
}

template<class distribution, class URNG>
void weight_noise(double **weights, int size, distribution &noise_dist, URNG &g) {
	std::uniform_int_distribution<int> neuron_dist(0, size - 1);

	for (int i = 0; i < size; i++) {
		weights[neuron_dist(g)][neuron_dist(g)] += noise_dist(g);
	}
}

#endif /*NOISE_H*/