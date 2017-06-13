import sys
import matplotlib
import matplotlib.pyplot as plt

matplotlib.rc('font', size=16)

for file in sys.argv[1:-1]:
	xs = []
	ys = []
	sigmas = []
	
	data = open(file, "r")
	for line in data:
		x, y, sigma = line.split()
		xs.append(float(x))
		ys.append(float(y))
		sigmas.append(float(sigma))
	data.close()

	try:
		plt.errorbar(xs, ys, yerr=sigmas, fmt="o", label=file.split('/')[-2])
	except IndexError:
		plt.errorbar(xs, ys, yerr=sigmas, fmt="o")

plt.xscale("log")
plt.yscale("log")

plt.legend()

plt.savefig(sys.argv[-1])
plt.close()