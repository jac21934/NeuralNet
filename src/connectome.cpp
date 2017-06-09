#include <cmath>

#include "connectome.h"

/**
 * Creates a new ConnectomeBuilder. This object will create neural networks with
 * the given parameters. Initial weights will be uniformly distributed
 * on [0, 1). Neurons' initial potentials will be 90% the firing threshold.
 *
 * @param size The number of Neurons that will populate Networks created by
 *  this object
 * @param fire_threshold The firing threshold of each Neuron
 * @param disfacilitation The disfacilitation of each Neuron
 * @param inhibitory_fraction The fraction of Neurons which will be inhibitory
 * @param output_fraction The fraction of Neurons which will not fire at
 *  threshold, acting as sinks for network activity
 * @param exponent The exponent of the scale-free distribution of neuron out
 *  degrees
 * @param g The source of randomness for network building
 */
ConnectomeBuilder::ConnectomeBuilder(
		int size,
		double fire_threshold,
		double disfacilitation,
		double inhibitory_fraction,
		double output_fraction,
		double exponent,
		RNG &g)
		: g(g)
		, size(size)
		, fire_threshold(fire_threshold)
		, disfacilitation(disfacilitation)
		, inhibit(inhibitory_fraction)
		, output(output_fraction)
		, weight_dist(0, 1)
		, neuron_dist(0, size - 1)
		, out_dist(std::min(2, size - 1), std::min(100, size - 1), exponent) {
	; // Nothing to see here
}

/**
 * Builds a network in the given vector. If the vector is already populated,
 * its contents will first be cleared.
 *
 * @param neurons The vector in which the Neurons will be placed
 * @param callback The function called by each Neuron when it reaches firing
 *  potential
 * @see Neuron::increase_potential(double)
 * @todo The power law distribution for out degrees could be achieved by
 *  preferential attachment. Would that be better?
 * @todo Should we actually give outgoing connections to output Neurons? I guess
 *  it doesn't really matter
 * @todo Technically, we should start over completely any time the RNG dictates
 *  that we should connect a Neuron to itself, or to a different Neuron
 *  more than once...
 */
void ConnectomeBuilder::operator()(
		std::vector<Neuron> &neurons,
		Neuron::ready_callback callback) {
	neurons.clear();
	/* The vector MUST NOT reallocate once we start adding vectors or a
	 * whole bunch of references are going to be invalidated
	 */
	neurons.reserve(size);

	for (int i = 0; i < size; i++) {
		neurons.emplace_back(output(g), inhibit(g), 0.9 * fire_threshold,
			fire_threshold, disfacilitation, callback);
	}

	for (auto it = neurons.begin(); it != neurons.end(); it++) {
		int index = std::distance(neurons.begin(), it);
		int remaining = out_dist(g);

		while (remaining > 0) {
			int receiver = neuron_dist(g);
			// No self-connections
			if (receiver == index)
				continue;

			// No strengthening existing connections
			try {
				it->get_connection_strength(neurons.at(receiver));
				continue;
			} catch (std::out_of_range e) {
				it->strengthen_connection(weight_dist(g), neurons.at(receiver));
				remaining--;
			}
		}
	}
}