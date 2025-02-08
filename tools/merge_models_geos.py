import sys, glob, os, subprocess


mil = []
with open("include/model_ids.h") as f:
    mil = f.readlines();

mtl = []
with open("actors/model_table.c") as f:
    mtl = f.readlines()

mid = []
mtd = {}

for l in mil:
    if "MODEL_" in l:
        mid.append(l.split(",")[0].strip())

for l in mtl:
    if "[MODEL_" in l:
        tok = l.split("=")
        name = tok[0].replace("[", "").replace("]","").strip()
        geo = tok[1].replace(",","").replace("\n","")

        p = subprocess.Popen(['git', 'grep', geo, 'actors'], stdout=subprocess.PIPE)
        matches = p.communicate()[0].decode('ascii').split("\n")
        folder = "_"
        for m in matches:
            if "GeoLayout" in m and "[] =" in m:
                folder = m.split(":")[0].split("/")[1]

        mtd[name] = [folder, geo]


with open("include/model_defines.h", "w+") as f:
    for k in mid:
        if k in mtd:
            f.write(f"DEFINE_MODEL({k}, {mtd[k][0]}, {mtd[k][1]})\n")
