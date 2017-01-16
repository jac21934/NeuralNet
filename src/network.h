#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <fstream>

#include "random_lib.h"

#define MIN_RES 0.0000001

class network {
public:
	// network_output.cpp
	void display(void);

	// network_input.cpp
	int input_net(const std::string filename);

	// network_init.cpp
	network(void);
	~network(void);

	// network.cpp
	void run(bool verbose);

private:
	// network_init.cpp
	void construct(void);
	void destruct(void);
	void out_degree_random(void);
	void out_degree_random(int edges);
	void out_degree_scale_free(double lambda);
	void out_degree_uniform(int degree);
	void set_out_degree(int i, int degree);
	void inhibitory_fraction(double fraction);

	// network_input.cpp
	void check_input(std::ifstream &inFile);

	// network.cpp
	void normalize_and_recount(void);
	void poke_random_neuron(void);

	double **weight;
	double *neuron;
	bool *refractory;
	int *out_degree;
	int *in_degree;
	int bond_number;

	int neurons;

	int max_turns;
	double fire_threshold;
	double inhibition;
	double transition;
	double noise_strength;

	bool initialized;

	Random rand;
};

#endif /*NETWORK_H*/