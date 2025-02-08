import os, sys, glob

actorfolders = ["actors/"+i.name for i in os.scandir("actors/") if os.path.isdir(i)]

headers = glob.glob("actors/*.h")
headers += glob.glob("levels/**/header.h")
hTable = {}
for h in headers:
    hl = []
    with open(h) as f:
        hl = f.readlines()
    hTable[h] = []
    for l in hl:
        if "extern const Collision" in l:
            tokens = l.split()
            hTable[h].append(tokens[3].split("[]")[0])
        if "extern const Gfx" in l:
            tokens = l.split()
            hTable[h].append(tokens[3].split("[]")[0])
        if "extern const GeoLayout" in l:
            tokens = l.split()
            hTable[h].append(tokens[3].split("[]")[0])


for i in actorfolders:
    files = os.listdir(i)
    fb = ""
    if "geo.inc.c" in files:
        with open(i+"/geo.inc.c", "r") as f:
            fb += f.read()

    # print(files)
    with open(i+"/data.c", "w+") as f:
        f.write('#include <PR/ultratypes.h>\n')
        f.write('#include "types.h"\n')
        if "model.inc.c" in files:
            f.write('#include <PR/mbi.h>\n') # for _SHIFTL (even though some compile fine without?)
            f.write('#include <PR/gbi.h>\n')
        f.write('\n')
        f.write('#include "macros.h"\n')
        if "geo.inc.c" in files:
            f.write('#include "geo_commands.h"\n')
        if "collision.inc.c" in files:
            f.write('#include "surface_terrains.h"\n')
        # f.write('#include "actor_types.h"\n')
        f.write('\n')

        for h in hTable:
            for k in hTable[h]:
                if k in fb:
                    f.write(f'#include "{h}"\n')

        f.write('\n')

        # table header

        if "model.inc.c" in files:
            f.write('#include "model.inc.c"\n')
        if "geo.inc.c" in files:
            f.write('#include "geo.inc.c"\n')
        if "anims" in files:
            f.write(f'#include "anims/data.inc.c"\n')
            f.write(f'#include "anims/table.inc.c"\n')
        if "collision.inc.c" in files:
            f.write('#include "collision.inc.c"\n')
