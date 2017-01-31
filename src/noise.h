#ifndef NOISE_H
#define NOISE_H

template<class distribution, class URNG>
void random_noise(double *neurons, int size, distribution &amplitude, URNG &g) {
	std::uniform_int_distribution<int> neuron_dist(0, size - 1);

	neurons[neuron_dist(g)] += amplitude(g);
}

#endif /*NOISE_H*/