import sys

binning = 0.5

final_dist = {}
files = 0

file_list = sys.argv[1:]
field = 2

try:
	field = int(sys.argv[1])
	file_list = sys.argv[2:]
except ValueError:
	pass

for file in file_list:
	files = files + 1
	data = open(file, "r")

	count = 0
	hist = {}
	for line in data:
		i = int(float(line.split()[field]) / binning)
		
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
	print binning * i, float(final_dist[i]) / files