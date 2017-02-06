#ifndef NETWORK_H
#define NETWORK_H

#include "network_param.h"

class Network {
public:
	// network_init.cpp
	Network(NetworkParams &params);
	~Network(void);

	// network.cpp
	void run();

private:
	// network.cpp
	void normalize_and_recount(void);

	double **weight;
	double *neuron;
	bool *refractory;
	int *out_degree;
	int *in_degree;
	int bond_number;

	int neurons;
	int max_turns;
	double fire_threshold;
	double disfacilitation;
	double transition;

	neuron_noise_gen nnoise;
	weight_noise_gen wnoise;
};

#endif /*NETWORK_H*/