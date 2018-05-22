#ifndef NETWORK_PARAM_H
#define NETWORK_PARAM_H

#include <string>
#include <fstream>
#include <functional>
#include <vector>
#include <memory>

#include "connectome.h"
#include "neuron.h"
#include "noise.h"

/**
 * Contains a number of parameters and objects used by Networks.
 */
class NetworkParams {
public:
	friend class Network;

	NetworkParams(const std::string &filename, RNG &g);
private:
	void parse_file(std::ifstream &inFile);

	RNG &g;
	bool psd;
	int delay;
	int size;
	int avalanches;
	int max_psd;
	int max_firings;
	int refractory_period;
	double fire_threshold;
	double disfacilitation;
	double max_strength;
	double inhibitory_fraction;
	double output_fraction;
	double exponent;
	double transition;
	double nnoise_mean;
	double nnoise_stdev;
	double wnoise_mean;
	double wnoise_stdev;

	std::shared_ptr<ConnectomeBuilder> builder;
	std::shared_ptr<NeuronNoise> nnoise;
	std::shared_ptr<WeightNoise> wnoise;
};

#endif /*NETWORK_PARAM_H*/
