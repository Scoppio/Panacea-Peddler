#!/usr/bin/python3

# written by Lucas Coppio 2020
# this program takes tmx output from Tiled
# and turns it into a .H file with arrays that can be included
# in NES C code for cc65

import sys
import os
import csv
from io import StringIO
import xml.etree.ElementTree as et


class Metatile(object):

    def __init__(self, tl, tr, bl, br, pal):
        self.tl = tl-1
        self.tr = tr-1
        self.bl = bl-1
        self.br = br-1
        self.pal = pal

    def __eq__(self, other):
        return self.tl == other.tl \
               and self.tr == other.tr \
               and self.bl == other.bl \
               and self.br == other.br \
               and self.pal == other.pal

    def __str__(self):
        return ",".join([str(self.tl), str(self.tr), str(self.bl), str(self.br), str(self.pal)])

    def __hash__(self):
        return self.tl*10000+self.tr*1000+self.bl*100+self.br*10+self.pal

    def __repr__(self):
        return self.__str__()


class MetatileSet(object):
    def __init__(self):
        self.metatiles = []
        self.hashtable = dict()

    def __add__(self, metatile: Metatile):
        if metatile not in self:
            self.metatiles.append(metatile)
            self.hashtable[hash(metatile)] = (len(self.metatiles) - 1, metatile)

        if len(self.metatiles) > 51:
            raise ValueError("ERROR - The room surpassed the maximum allowed number of 51 metatiles!")

        return self.hashtable[hash(metatile)][0]

    def __contains__(self, item):
        return hash(item) in self.hashtable

    def __str__(self):
        return "\n".join([str(i) for i in self.metatiles])


def make_room(name, data):
    print("Parsing room", name)
    m, r = make_metatiles_and_room(data)
    return print_room_and_metatile(name, m, r)


def print_room_and_metatile(name, metatileset, room):
    doc = "const unsigned char " + name + "_mt[]={\n"
    doc += ",\n".join([str(m) for m in metatileset.metatiles])
    doc += "\n};\n\n"
    doc += "const unsigned char " + name + "[]={\n"
    lines = []
    for line in room:
        lines.append(",".join([str(i) for i in line]))
    doc += ",\n".join(lines)
    doc += "\n};\n\n"

    return doc


def make_metatiles_and_room(matrix):
    rows = int(len(matrix))
    columns = int(len(matrix[0]))
    metatileset = MetatileSet()
    room = []

    for line in range(0, rows, 2):
        roomline = []
        for col in range(0, columns, 2):
            tl = int(matrix[line][col])
            tr = int(matrix[line][col + 1])
            bl = int(matrix[line + 1][col])
            br = int(matrix[line + 1][col + 1])
            m = Metatile(tl, tr, bl, br, 0)
            idx = metatileset + m
            roomline.append(idx)
        room.append(roomline)

    print("metatiles | {metatiles}".format(metatiles=len(metatileset.metatiles)))

    return metatileset, room


if __name__ == "__main__":
    filename = sys.argv[1]
    outputfile_name = filename[0:-4] + ".h"
    outputfile = sys.argv[2]
    root = et.parse(filename).getroot()
    with open(outputfile, 'w') as writer:
        for type_tag in root.findall('layer'):
            name = type_tag.attrib["name"]
            data = type_tag.find('data')
            v = data.text[1:].replace(",\n", "\n")
            f = StringIO(v)
            l = list(csv.reader(f))
            try:
                s = make_room("room_" + name, l)
            except Exception as e:
                print(e)
                writer.close()
                os.remove(outputfile)
                raise e

            writer.write(s)

    exit(0)
