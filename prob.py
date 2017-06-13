import sys
import math

binning = 1.0 / 20
avg_shift = (1.0 + 10 ** binning) / 2

file_list = sys.argv[1:]
field = 2

def x(i):
	return 10**(binning * i) * avg_shift

try:
	field = int(sys.argv[1])
	file_list = sys.argv[2:]
except ValueError:
	pass

stats = {}
for file in file_list:
	data = open(file, "r")

	run_hist = {}
	events = 0

	for line in data:
		raw = float(line.split()[field])
		if raw == 0:
			continue
		try:
			i = int(math.floor(math.log10(raw) / binning))
		except ValueError:
			raise ValueError("Error: file %s field %d line %s" % (file, field, line))
		
		events = events + 1

		try:
			run_hist[i] = run_hist[i] + 1
		except KeyError:
			run_hist[i] = 1
	data.close()

	for i in run_hist:
		# y value for this datapoint, corrected for variable bin widths
		y = (float(run_hist[i]) / events) / (binning * math.log(10) * x(i))
		try:
			stats[i][0] = stats[i][0] + 1
			stats[i][1] = stats[i][1] + y
			stats[i][2] = stats[i][2] + y**2
		except KeyError:
			stats[i] = [1, y, y**2]

for i in stats:
	mu = stats[i][1] / stats[i][0]
	try:
		sigma = math.sqrt((stats[i][2] / stats[i][0] - mu**2) * stats[i][0] / (stats[i][0] - 1))
	except ValueError:
		# rounding error
		continue
	except ZeroDivisionError:
		# too few samples
		continue
	print x(i), mu, sigma