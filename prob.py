import sys
import math

binning = 1.0 / 10
avg_shift = (1.0 + 10 ** binning) / 2

file_list = sys.argv[1:]
field = 2

try:
	field = int(sys.argv[1])
	file_list = sys.argv[2:]
except ValueError:
	pass

count = 0
hist = {}
for file in file_list:
	data = open(file, "r")

	for line in data:
		raw = float(line.split()[field])
		if raw == 0:
			continue
		try:
			i = int(math.floor(math.log10(raw) / binning))
		except ValueError:
			raise ValueError("Error: file %s field %d line %s" % (file, field, line))
		
		count = count + 1

		try:
			hist[i] = hist[i] + 1
		except KeyError:
			hist[i] = 1
	data.close()

for i in hist:
	# The average value of this bin
	x = 10**(binning * i) * avg_shift
	# Adjust for variable bin widths
	y = (float(hist[i]) / count) / binning / math.log(10) / x
	print x, y