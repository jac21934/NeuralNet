#include <cmath>
#include <climits>

#include "network_param.h"

/**
 * Creates a new NetworkParams object by parsing a configuration file
 *
 * @param filename The name of the configuration file
 * @param g The RNG to act as the source of randomness for all network purposes
 */
NetworkParams::NetworkParams(const std::string &filename, RNG &g)
		: g(g)
		, size(0)
		, avalanches(0)
		, max_firings(INT_MAX)
		, refractory_period(1)
		, fire_threshold(0)
		, disfacilitation(0)
		, max_strength(0)
		, inhibitory_fraction(0)
		, output_fraction(0)
		, exponent(0)
		, transition(0)
		, nnoise_mean(0)
		, nnoise_stdev(0)
		, wnoise_mean(0)
		, wnoise_stdev(0)
		, builder()
		, nnoise()
		, wnoise() {
	std::ifstream inFile(filename.c_str());

	if (!inFile)
		throw std::runtime_error("No file with name \"" + filename + "\"");

	while (inFile)
		parse_file(inFile);

	inFile.close();

	builder = std::make_shared<ConnectomeBuilder>(size, max_firings,
		refractory_period, fire_threshold, disfacilitation, max_strength,
		inhibitory_fraction, output_fraction, exponent, g);
	nnoise = std::make_shared<NeuronNoise>(nnoise_mean, nnoise_stdev, g);
	wnoise = std::make_shared<WeightNoise>(wnoise_mean, wnoise_stdev, g);
}

/**
 * Extracts the value of the given parameter from a single line from the
 * configuration file. If the line in question does not contain a setting for
 * the desired parameter, NaN is returned.
 *
 * @param line The line of text being searched for the desired parameter
 * @param parameter The parameter being searched for
 * @return The value to which the parameter was set if present, NaN otherwise
 */
static double get_parameter(std::string line, std::string parameter) {
	double ret = NAN;

	if (line.substr(1, parameter.length() + 1) == parameter + ":")
		ret = std::atof(line.substr(parameter.length() + 2).c_str());

	return ret;
}

/**
 * Populates a number of member variables with values found in the passed file.
 *
 * @param inFile File from which configuration values will be read
 * @todo A more elegant parser would be nice
 */
void NetworkParams::parse_file(std::ifstream &inFile) {
	std::string line;
	std::getline(inFile, line);

	if(line[0] != '#')
		return;

	double value;
	if (!std::isnan(value = get_parameter(line, "NET_SIZE"))) {
		size = value;
	} else if (!std::isnan(value = get_parameter(line, "FIRE_THRESHOLD"))) {
		fire_threshold = value;
	} else if (!std::isnan(value = get_parameter(line, "DISFACILITATION"))) {
		disfacilitation = value;
	} else if (
			!std::isnan(value = get_parameter(line, "TRANSITION_THRESHOLD"))) {
		transition = value;
	} else if (!std::isnan(value = get_parameter(line, "AVALANCHES"))) {
		avalanches = value;
	} else if (!std::isnan(value = get_parameter(line, "NEURON_NOISE_MEAN"))) {
		nnoise_mean = value;
	} else if (!std::isnan(value = get_parameter(line, "NEURON_NOISE_STDEV"))) {
		nnoise_stdev = value;
	} else if (!std::isnan(value = get_parameter(line, "WEIGHT_NOISE_MEAN"))) {
		wnoise_mean = value;
	} else if (!std::isnan(value = get_parameter(line, "WEIGHT_NOISE_STDEV"))) {
		wnoise_stdev = value;
	} else if (!std::isnan(value = get_parameter(line, "EXPONENT"))) {
		exponent = value;
	} else if (!std::isnan(value = get_parameter(line, "INHIBITORY"))) {
		inhibitory_fraction = value;
	} else if (!std::isnan(value = get_parameter(line, "OUTPUT"))) {
		output_fraction = value;
	} else if (!std::isnan(value = get_parameter(line, "MAX_STRENGTH"))) {
		max_strength = value;
	} else if (!std::isnan(value = get_parameter(line, "MAX_FIRINGS"))) {
		max_firings = value;
	} else if (!std::isnan(value = get_parameter(line, "REF_PERIOD"))) {
		refractory_period = value;
	} else {
		throw std::runtime_error("Unrecognized option: \"" + line + "\"");
	}
}
