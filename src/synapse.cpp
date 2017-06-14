#include <cmath>

#include "neuron.h"

/**
 * Creates a new synapse with the given strength between the given neurons. Will
 * increase the postsynaptic neuron's in_degree and the presynaptic neuron's
 * out_degree.
 *
 * @param from Neuron which transmits charge along this synapse
 * @param to Neuron which receives charge sent down this synapse
 * @param initial_strength The initial strength of this connection
 * @throws 
 */
Neuron::Synapse::Synapse(Neuron &from, Neuron &to, double initial_strength)
		: from(from)
		, to(to)
		, strength(initial_strength)
		, accumulated_charge(0) {

	if (initial_strength < MIN_RES)
		throw std::invalid_argument("Initial strength too low");

	to.inc_in_degree();
}

/**
 * Destructor. Decrements presynaptic neuron's out_degree and postsynaptic
 * neuron's in_degree.
 */
Neuron::Synapse::~Synapse(void) {
	to.dec_in_degree();
}

/**
 * Resets all charge accumulated since last call to either this or
 * hebbian_increase without changing strength.
 *
 * @see Synapse::fire(double)
 * @see Synapse::hebbian_increase()
 */
void Neuron::Synapse::reset(void) {
	accumulated_charge = 0;
}

/**
 * Sends charge to postsynaptic Neuron. Amount by which the postsynaptic
 * Neuron's potential is increased is recorded.
 *
 * @return Postsynaptic Neuron potential increase
 * @see Synapse::hebbian_increase()
 * @see Synapse::reset();
 */
double Neuron::Synapse::fire(void) {
	double sent = to.increase_potential(from.get_potential()
		* from.get_out_degree() * strength * from.get_character()
		/ to.get_in_degree() / from.get_weight_sum(), true);

	accumulated_charge += std::fabs(sent);
	return sent;
}

/**
 * Increase strength by the supplied amount.
 *
 * @param delta Amount by which to increase bond strength
 * @return True if the synapse needs to be pruned
 */
bool Neuron::Synapse::increase_strength(double delta) {
	strength += delta;

	return strength < MIN_RES;
}

/**
 * Apply Hebbian learning rules based on the charge accumulated since last call
 * to either this or reset.
 *
 * @param rate Learning rate
 * @return Amount by which this connection was increased
 * @see Synapse::fire(double)
 * @see Synapse::reset(void)
 */
double Neuron::Synapse::hebbian_increase(double rate) {
	double delta = rate * accumulated_charge / from.get_threshold();

	increase_strength(delta);
	reset();

	return delta;
}