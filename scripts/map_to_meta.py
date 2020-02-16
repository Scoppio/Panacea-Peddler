#!/usr/bin/python3

# written by Doug Fraker 2018
# this program takes csv output from Tiled
# and turns it into a C style array that can be included
# NES programming...cc65

import sys
import csv
import os


class Metatile(object):

	def __init__(self, tl, tr, bl, br, pal):
		self.tl = tl
		self.tr = tr
		self.bl = bl
		self.br = br
		self.pal = pal

	def __eq__(self, other):
		return self.tl == other.tl and self.tr == other.tr and self.bl == other.bl and self.br == other.br

	def __str__(self):
		return ", ".join([str(self.tl), str(self.tr), str(self.bl), str(self.br), str(self.pal)])

	def __hash__(self):
		return hash(self.__str__())

	def __repr__(self):
		return self.__str__()


filename = sys.argv[1]
newname = filename[0:-4] + "_mt.c"
newname2 = os.path.basename(filename)
newname2 = newname2[0:-4]+"_mt"

with open(filename, 'r') as oldfile:
	reader = csv.reader(oldfile)
	your_list = list(reader)

with open(newname, 'w') as newfile:
	newfile.write("const unsigned char " + newname2 + "[]={\n")

	rows = len(your_list)
	columns = len(your_list[0])

	metatiles = set()

	for i in range (0, rows, 2):
		for j in range (0, columns, 2):
			tl = your_list[i][j]
			tr = your_list[i][j+1]
			bl = your_list[i+1][j]
			br = your_list[i+1][j+1]
			pal = 0
			metatiles.add(Metatile(tl, tr, bl, br, 0))

	if len(metatiles) > 51:
		print("Error, map has a total of " + str(len(metatiles)) + ", the maximum is 51.")
		exit(0)

	for metatile in metatiles:
		newfile.write(str(metatile) + ",\n")

	# delete that last comma, back it up
	z = newfile.tell()
	z = z - 3
	newfile.seek(z)

	newfile.write("\n};\n\n")

print("Done.")