#ifndef NETWORK_PARAM_H
#define NETWORK_PARAM_H

#include <string>
#include <fstream>
#include <random>
#include <functional>

#define MIN_RES 0.0001

typedef std::mt19937 RNG;

typedef std::function<void(double *, int, bool, double)> neuron_noise_gen;
typedef std::function<void(double **, int)> weight_noise_gen;
typedef std::function<void(double **, int*, bool*, int)> connectome_builder;

class NetworkParams {
public:
	friend class Network;

	NetworkParams(const std::string &filename, RNG &g);
private:
	void parse_file(std::ifstream &inFile);

	RNG &g;
	int size;
	int avalanches;
	double fire_threshold;
	double disfacilitation;
	double transition;
	connectome_builder builder;
	neuron_noise_gen nnoise;
	weight_noise_gen wnoise;
};

#endif /*NETWORK_PARAM_H*/