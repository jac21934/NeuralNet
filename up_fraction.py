import sys

file_list = sys.argv[1:]
field = 6

up = 0
total = 0
for file in file_list:
	data = open(file, "r")

	for line in data:
		total = total + 1		
		if line.split()[field] == "1":
			up = up + 1

	data.close()

print "%d avalanches, %f%% up states" % (total, 100.0 * up / total)
