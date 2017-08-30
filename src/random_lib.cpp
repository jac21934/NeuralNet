#include <climits>
#include <limits>
#include <cstdlib>
#include <cmath>

#include "random_lib.h"

Random::Random(void) {
	for (int i = 0; i < 16; i++) {
		state[i] = rand();
	}

	index = 0;

	generate = false;
}

unsigned long Random::WELL512(void) {
	unsigned long a, b, c, d;

	a = state[index];
	c = state[(index + 13) & 15];
	b = a ^ c ^ (a << 16) ^ (c << 15);
	c = state[(index + 9) & 15];
	c ^= (c >> 11);
	state[index] = b ^ c;
	a = b ^ c;
	d = a ^ ((a << 5) & 0xDA442D24UL);

	index = (index + 15) & 15;

	a = state[index];

	state[index] = a ^ b ^ d ^ (a << 12) ^ (b << 18) ^ (c << 28);

	return state[index];
}

double Random::random_real(void) {
	return (double) WELL512() / ULONG_MAX;
}

double Random::random_gauss(double mean, double stdev) {
	const double epsilon = std::numeric_limits<double>::min();
	const double two_pi = 2.0 * 3.14159265358979323846;

	generate = !generate;

	if (!generate)
		return z1 * stdev + mean;

	double u1;
	double u2;
	do {
		u1 = random_real();
		u2 = random_real();
	} while (u1 <= epsilon);

	double z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);

	return z0 * stdev + mean;
}

double Random::random_interval(double a, double b) {
	return random_real() * (b - a) + a;
}

int Random::random_int(void) {
	return WELL512() % INT_MAX;
}

unsigned int Random::random_uint(void) {
	return WELL512() % UINT_MAX;
}

long Random::random_long(void) {
	return WELL512() % LONG_MAX;
}

unsigned long Random::random_ulong(void) {
	return WELL512() % ULONG_MAX;
}
