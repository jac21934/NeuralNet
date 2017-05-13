import sys
import matplotlib
import matplotlib.pyplot as plt

matplotlib.rc('font', size=16)

for file in sys.argv[1:-1]:
	xs = []
	ys = []
	
	data = open(file, "r")
	for line in data:
		x, y = line.split()
		xs.append(x)
		ys.append(y)
	data.close()

	plt.plot(xs, ys, "o", label='/'.join(file.split('/')[-2:-1]))

plt.xscale("log")
plt.yscale("log")

plt.legend()

plt.savefig(sys.argv[-1])
plt.close()