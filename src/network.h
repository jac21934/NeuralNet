#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <memory>

#include "neuron.h"
#include "noise.h"
#include "network_param.h"

/**
 * Represents an actual network full of neurons. Orchestrates Neuron firings
 * and collects and reports statistics.
 */
class Network {
public:
		Network(NetworkParams &params);

		void run(std::ostream &out);
private:
		std::vector<Neuron> neurons;
		const unsigned int avalanches;
		const int max_psd;
		const unsigned int delay;
		const double transition;
		const bool psd;

		//variables for neuron suppression
		const double suppress_chance;
		const int suppress_type;
		const unsigned int suppress_delay;
		
		
		bool ready_to_fire;

		std::shared_ptr<NeuronNoise> nnoise;
		std::shared_ptr<WeightNoise> wnoise;


		int apply_noise_until_ready(void);
		int apply_noise_until_ready(double last_depol);
		void neuron_callback(Neuron &n);
};

#endif /*NETWORK_H*/
