#ifndef NOISE_H
#define NOISE_H

#include <vector>
#include <random>

#include "neuron.h"

typedef std::mt19937 RNG;

/**
 * Creates and injects noise into the potentials of Neurons in a Network.
 */
class NeuronNoise {
public:
	NeuronNoise(double mean, double stdev, RNG &g);

	void operator()(std::vector<Neuron> &neurons, double range);
	void operator()(std::vector<Neuron> &neurons);

private:
	RNG &g;
	std::normal_distribution<double> down_dist;
};

/**
 * Injects noise into the connection strengths between Neurons in a Network.
 */
class WeightNoise {
public:
	WeightNoise(double mean, double stdev, RNG &g);

	void operator()(std::vector<Neuron> &neurons);

private:
	RNG &g;
	std::normal_distribution<double> noise_dist;
};

#endif /*NOISE_H*/