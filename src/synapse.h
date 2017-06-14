#ifndef SYNAPSE_H
#define SYNAPSE_H

#define MIN_RES 0.0001

class Neuron;

/**
 * A weighted connection between neurons. Keeps track of all charge which passes
 * through itself.
 */
class Synapse {
public:
	Synapse(Neuron &from, Neuron &to, double initial_strength);
	~Synapse(void);

	void reset(void);
	double fire(double potential);
	bool increase_strength(double delta);
	double hebbian_increase(double rate);
	double get_strength(void) const { return strength; }
private:
	Neuron &from;
	Neuron &to;
	double strength;

	double accumulated_charge;
};

#endif /*SYNAPSE_H*/