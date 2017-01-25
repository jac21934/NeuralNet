#ifndef POWER_LAW_H
#define POWER_LAW_H

#include <limits>
#include <cmath>
#include <random>

template <class IntType = int>
class power_law_distribution {
public:
	typedef IntType result_type;

	power_law_distribution(result_type a = 1,
		result_type b = std::numeric_limits<result_type>::max(), double exp = 2);

	void reset(void) { }

	template <class URNG>
	result_type operator()(URNG &g);

	result_type min(void);
	result_type max(void);

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
		double exp) {

	this->a = a;
	this->b = b;
	this->exp = exp;

	normalization = 0;

	for (result_type i = a; i <= b; i++) {
		normalization += pow(i, -exp);
	}
}

template <class IntType>
template <class URNG>
typename power_law_distribution<IntType>::result_type 
		power_law_distribution<IntType>::operator()(URNG &g) {

	std::uniform_real_distribution<double> uniform;
	double prob = uniform(g);

	double cumulative = 0;
	result_type ret;
	for (ret = a; ret < b; ret++) {
		cumulative += pow(ret, -exp) / normalization;
		if (cumulative > prob)
			break;
	}

	return ret;
}

template <class IntType>
typename power_law_distribution<IntType>::result_type 
		power_law_distribution<IntType>::min(void) {
	return a;
}

template <class IntType>
typename power_law_distribution<IntType>::result_type 
		power_law_distribution<IntType>::max(void) {
	return b;
}

#endif /*POWER_LAW_H*/