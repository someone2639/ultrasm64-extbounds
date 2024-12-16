import os

fl = []
with open("data/behavior_data.c", "r") as f:
    fl = f.readlines()

bhvTable = {}

actdirs = [i.name for i in os.scandir("actors") if os.path.isdir(i)]

def lookahead(fl, i):
    d = i
    while fl[d] != "};\n":
        d += 1
    return d

for i, line in enumerate(fl):
    if "const BehaviorScript" in line:
        tokens = line.split()
        bname = tokens[2].replace("[]","")
        if bname.lower().replace("bhv","") in actdirs:
            bhvTable[bname] = fl[i:lookahead(fl, i)]

print(bhvTable)
