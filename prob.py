import sys

final_dist = {}
files = 0
for file in sys.argv[1:]:
	files = files + 1
	data = open(file, "r")

	count = 0
	hist = {}
	for line in data:
		i = int(line)
		
		count = count + 1

		try:
			hist[i] = hist[i] + 1
		except KeyError:
			hist[i] = 1

	for i in hist:
		try:
			final_dist[i] = final_dist[i] + float(hist[i]) / count
		except KeyError:
			final_dist[i] = float(hist[i]) / count

for i in final_dist:
	print i, float(final_dist[i]) / files