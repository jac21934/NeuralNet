/* Technically, we shouldn't be handling random numbers like this at all, but
 * doing it correctly requires a lot of care and a lot of throwing attempts away
 * and it just doesn't seem worth it
 */

#ifndef RANDOM_LIB_H
#define RANDOM_LIB_H

class Random {
public:
	double random_real(void);
	double random_gauss(double mean, double stdev);
	double random_interval(double a, double b);

	int random_int(void);
	unsigned int random_uint(void);

	long random_long(void);
	unsigned long random_ulong(void);

	Random(void);

private:
	unsigned long WELL512(void);

	unsigned long state[16];
	unsigned long index;

	bool generate;
	double z1;
};

#endif /*RANDOM_LIB_H*/