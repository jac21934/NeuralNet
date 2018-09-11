#include <iostream>
#include <functional>
#include <iomanip>      // std::setprecision

#include "network.h"

/**
 * Creates a new Network. This isn't a great description, but what do you want
 * from me?
 *
 * @param params NetworkParams object containing parameters for the Network
 */
Network::Network(NetworkParams &params)
		: neurons()
		, avalanches(params.avalanches)
		, max_psd(params.max_psd)
		, delay(params.delay)
		, transition(params.transition)
		, psd(params.psd)
		, suppress_chance(params.suppress_chance)
		, suppress_type(params.suppress_type)
		, suppress_delay(params.suppress_delay)
		, ready_to_fire(false)
		, nnoise(params.nnoise)
		, wnoise(params.wnoise) {
		// Is this moral?
		(*params.builder)(neurons,
											std::bind(&Network::neuron_callback, this, std::placeholders::_1));
}

/**
 * Actually runs the simulation until the desired number of avalanches has
 * occurred.
 *
 * @param out Stream to which avalanche information will be sent
 */
void Network::run(std::ostream &out) {
		bool is_up = false;
		double last_depol_sum = 0;
		int psd_count = 0;

//		double suppressChance = 0.00001;
		// double suppressAmount = 10.0;

		for (unsigned int i = 0; i < avalanches; i++) {
				int wait_time = 0;
				double depol_sum = 0;
				int duration;
				while (depol_sum == 0) {
						// Apply noise until we're ready to fire
						for ( ; !ready_to_fire; wait_time++) {
								if (is_up) {
										(*nnoise)(neurons, transition / last_depol_sum);
								} else {
										(*nnoise)(neurons);
								}
						}

						//Put control here

						// if we're past the delay, we chose neurons of our designated
						// type and roll the dice to see if they get suppressed.
						if(suppress_delay == i){
								nnoise->PickSuppressedNeurons(neurons, suppress_chance, suppress_type);
						}

						
						// Avalanche time
						for (duration = 0; ready_to_fire; duration++) {
								ready_to_fire = false;

								// Prepare all neurons for firings
								for (auto it = neurons.begin(); it != neurons.end(); it++) {
										it->prepare();
								}
												
								// Fire if/when ready
								double eeg = 0;
								for (auto it = neurons.begin(); it != neurons.end(); it++) {
										double depol = it->time_step();
										depol_sum += std::fabs(depol);
										eeg += depol;
								}

								if (psd && i >= delay && depol_sum != 0) {
										out << eeg <<  '\t';
										if(++psd_count >= max_psd) {
												i = avalanches;
												break;
										}
								}
						}
				}
				last_depol_sum = depol_sum;

				// Hebbian learning
				double total_weight_increase = 0;
				int pre_lower_bond_number = 0;
				for (auto it = neurons.begin(); it != neurons.end(); it++) {
						total_weight_increase += it->hebbian();
						pre_lower_bond_number += it->get_out_degree();
				}

				// Homeostatic weight lowering
				for (auto it = neurons.begin(); it != neurons.end(); it++) {
						it->strengthen_all_connections(-total_weight_increase
																					 / pre_lower_bond_number);
				}

				// Reporting
				if (i >= delay) {
						if (psd)
								out << '\n';
						out << std::setprecision(6);
						out << std::setw(10) << depol_sum << '\t'
								<< std::setw(10) << duration << '\t' << std::endl;

				}

				// Up/down transition
				is_up = false;
				for (auto it = neurons.begin(); it != neurons.end(); it++) {
						if (is_up)
								it->go_up(it->get_threshold() * (1 - depol_sum / transition));
						else
								it->go_down();
				}

		}
}

/**
 * Called by constituent neurons when they're ready to fire.
 *
 * @param n Neuron which is ready to fire
 * @see Neuron::Neuron(bool, bool, double, double, ready_callback)
 */
void Network::neuron_callback(Neuron &n) {
		ready_to_fire = true;
}
