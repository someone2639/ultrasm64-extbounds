import sys

# Usage:
#  getGoddardSize.py <build_dir/> <.map file>

with open(sys.argv[2]) as f:
	for line in f:
		if "GODDARD_SIZE" in line:
			tokens=line.split()
			with open("%s/goddard.txt" % sys.argv[1], "w+") as f:
				sz = int(tokens[0], 16)
				sz += 16
				sz &= 0xFFFFFFF0
				f.write("GODDARD_SIZE = 0x%X;" % sz)
