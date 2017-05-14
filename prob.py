import sys

binning = 1

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
		i = int(float(line.split()[field]) / binning)
		
		count = count + 1

		try:
			hist[i] = hist[i] + 1
		except KeyError:
			hist[i] = 1
	data.close()

for i in hist:
	print binning * i, float(hist[i]) / count