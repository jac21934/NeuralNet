import sys
from scipy import signal
import matplotlib.pyplot as plt

file_list = sys.argv[1:]
bins = 256

eeg = []
for filename in file_list:
	file = open(filename, "r")

	k = True
	for line in file:
		k = not k
		if k:
			continue

		eeg.extend([float(x) for x in line.split()])
	file.close()

# Welch's method already averages, so we can just pile them all together
f, psd = signal.welch(eeg, nperseg=bins)

#	plt.plot(f, psd, "r.")
#	plt.xscale("log")
#	plt.yscale("log")
#	plt.title(filename)
#	plt.show()

for i in range(len(f)):
	print f[i], psd[i]
