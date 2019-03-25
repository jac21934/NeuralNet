import sys
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
matplotlib.rc('font', size=16)
from scipy.optimize import curve_fit

def func(x, a, b):
        return a * np.power(x,-b)


# minData = 20
# maxData = 100
minData = 20
maxData = 100


fig, ax = plt.subplots()
ax.set_xscale("log")
ax.set_yscale("log")


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

	line1, = ax.plot(xs, ys, "o", label="Data")

        popt, pcov = curve_fit(func, xs[minData:maxData], ys[minData:maxData])
        print popt

        fitData = []
        for x in xs:
                fitData.append(func(x, popt[0], popt[1]))
                
        fitLegend = "$S^{-" + str(round(popt[1],2)) +  "}$"
        fitLine, = ax.plot(xs, fitData, label=fitLegend )
        ax.legend([line1, fitLine])

# plt.xscale("log")
# plt.yscale("log")
ax.set_xlabel("S", fontsize=20)
ax.set_ylabel("P(S)", fontsize=20)
# ax.set_xlim(7,)


# plt.xlabel("$P(S)$", fontsize=20)
# plt.ylabel("$S$", fontsize=20)

ax.set_ylim(0.00000001,10);

ax.legend()
plt.tight_layout()


plt.savefig(sys.argv[-1])
plt.close()
