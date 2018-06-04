import sys
import math

binning = 1.0 / 20

file_list = sys.argv[1:]
field = 2

# First option for linear binning, second for logarithmic
def xofi(i):
	#return (i + 0.5) * binning
	return 10**(binning * i) * (1.0 + 10 ** binning) / 2
def iofx(x):
	#return int(math.floor(x / binning))
	return int(math.floor(math.log10(x) / binning))
def binwidth(i):
	#return binning
	return binning * math.log(10) * xofi(i)

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

	k = False
	for line in data:
		k = not k
		if k:
			continue

		raw = float(line.split()[field])
		if raw == 0:
			continue
		try:
			i = iofx(raw)
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
		y = (float(run_hist[i]) / events) / binwidth(i)
		try:
			stats[i][0] = stats[i][0] + 1
			stats[i][1] = stats[i][1] + y
			stats[i][2] = stats[i][2] + y**2
		except KeyError:
			stats[i] = [1, y, y**2]

for i in stats:
	mu = stats[i][1] / stats[i][0]
	"""try:
		sigma = math.sqrt((stats[i][2] / stats[i][0] - mu**2) * stats[i][0] / (stats[i][0] - 1))
	except ValueError:
		# rounding error
		continue
	except ZeroDivisionError:
		# too few samples
		continue"""
	print xofi(i), mu#, sigma
