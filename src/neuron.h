#ifndef NEURON_H
#define NEURON_H

#include <unordered_map>
#include <atomic>
#include <functional>

#define MIN_RES 0.0001

/**
 * A single neuron. Keeps track of potential, outgoing connections to other
 * neurons, and some recent history. Fires when potential is reached. Handles
 * details of Hebbian plasticity.
 */
class Neuron {
public:
	typedef std::function<void(Neuron&)> ready_callback;

	Neuron(
		bool output, 
		bool inhibitory, 
		double initial_potential, 
		double fire_threshold, 
		double disfacilitation, 
		ready_callback ready_to_fire);

	double increase_potential(double delta, bool record = false);
	double time_step(void);
	double hebbian(double rate = 1);
	void strengthen_connection(double delta, Neuron &target);
	void strengthen_all_connections(double delta);
	double get_weight_sum(void) const;
	double get_connection_strength(Neuron &target) const;
	void go_up(double potential);
	void go_down();

	void prepare(void);

	int get_in_degree(void) const;
	int get_out_degree(void) const;
	int get_id(void) const;
	int get_character(void);
	double get_threshold(void) const;
	bool was_active(void) const;
private:
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
	friend Synapse;

	static std::atomic_int unused_id;

	const int id;
	const double threshold;
	const double disfacilitation;
	const int character;
	const bool is_out;

	double next_potential;
	double current_potential;
	double depol;
	double weight_sum;
	bool next_refractory;
	bool current_refractory;
	bool active;

	int in_degree;

	ready_callback callback;

	std::unordered_map<int, Synapse> synapses;
	void renormalize_weights(void);
	void inc_in_degree(void) { in_degree++; }
	void dec_in_degree(void) { in_degree--; }
};

#endif /*NEURON_H*/