import sys
import math
import numpy as np

"""
stats[i][0] is a count of additions
stats[i][1] is the sum of powers
stats[i][2] is the sum of squares
"""

file_list = sys.argv[1:]
binning = 5.0 * 10**(-3)

# First option for linear binning, second for logarithmic
def xofi(i):
	return (i + 0.5) * binning
	#return 10**(binning * i) * (1.0 + 10**binning) / 2
def iofx(x):
	return int(math.floor(x / binning))
	#return int(math.floor(math.log10(x) / binning))
def binwidth(i):
	return binning
	#return binning * math.log(10) * xofi(i)


stats = {}
for filename in file_list:
	file = open(filename, "r")

	eeg = []
	k = True
	for line in file:
		k = not k
		if k:
			continue

		eeg.extend([float(x) for x in line.split()])
	file.close()

	l = (len(eeg) / 10000) * 10000
	f = np.fft.rfftfreq(l)
	psd = np.abs(np.fft.rfft(eeg, l)) ** 2.0

	for n in range(1, len(psd)):
		i = iofx(f[n])
		y = psd[n] / binwidth(i)
		try:
			stats[i][0] = stats[i][0] + 1
			stats[i][1] = stats[i][1] + y
			stats[i][2] = stats[i][2] + y ** 2
		except KeyError:
			stats[i] = [1, y, y**2]

for i in sorted(stats):
	mu = stats[i][1] / stats[i][0]
	"""try:
		sigma = math.sqrt((stats[i][2] / stats[i][0] - mu ** 2) * stats[i][0] / (stats[i][0] - 1))
	except ValueError:
		# rounding error
		continue
	except ZeroDivisionError:
		# too few samples
		sigma = float("inf")"""
	print xofi(i), mu#, sigma
