#ifndef POWER_LAW_H
#define POWER_LAW_H

#include <limits>
#include <cmath>
#include <random>

template <class IntType = int>
class power_law_distribution {
public:
	typedef IntType result_type;

	power_law_distribution(
		result_type a = 1,
		result_type b = std::numeric_limits<result_type>::max(),
		double exp = 2);

	void reset(void) { ; }

	template <class URNG>
	result_type operator()(URNG &g);

	result_type min(void) { return a; }
	result_type max(void) { return b; }
private:
	result_type a;
	result_type b;
	double exp;
	double normalization;
};

template <class IntType>
power_law_distribution<IntType>::power_law_distribution(
		result_type a,
		result_type b,
		double exp)
		: a(a)
		, b(b)
		, exp(exp)
		, normalization(0) {
	for (result_type i = a; i < b; i++) {
		normalization += std::pow(i, -exp);
	}
}

template <class IntType>
template <class URNG>
typename power_law_distribution<IntType>::result_type
		power_law_distribution<IntType>::operator()(URNG &g) {
	std::uniform_real_distribution<double> uniform;
	double prob = uniform(g);

	double cumulative = 0;
	result_type ret = a;
	while (ret < b) {
		cumulative += std::pow(ret, -exp) / normalization;
		if (cumulative > prob)
			break;
		ret++;
	}

	return ret;
}

#endif /*POWER_LAW_H*/