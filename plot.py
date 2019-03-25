import sys
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
matplotlib.rc('font', size=16)

for file in sys.argv[1:-1]:
	xs = []
	ys = []
	sigmas = []
	
	data = open(file, "r")
	for line in data:
		x, y = line.split()
		xs.append(float(x))
		ys.append(float(y))
		#sigmas.append(float(sigma))
	data.close()
        print xs
	plt.errorbar(xs, ys, fmt="o" )
                
plt.xscale("log")
plt.yscale("log")

plt.xlabel("$P(S)$", fontsize=20)
plt.ylabel("$S$", fontsize=20)

plt.tight_layout()

plt.legend()

plt.savefig(sys.argv[-1])
plt.close()
