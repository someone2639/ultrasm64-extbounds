import os, re, glob


n2snake = re.compile(r'(?<!^)(?=[A-Z0-9])')
levelname = re.compile(r'[A-Z]{3}')

def to_snake_case(name):
    # print(name)
    levels = re.findall(levelname, name)
    if "SL" in name[:2]:
        print(name)
        name = name.replace("SL", "Sl")
    elif (len(levels) > 0):
        name = name.replace(levels[0],levels[0].lower())
    return n2snake.sub('_', name).lower()

fl = []
with open("data/behavior_data.c", "r") as f:
    fl = f.readlines()

bhvTable = {}

actdirs = [i.replace(".c","") for i in os.listdir("src/game/behaviors")]

# print(actdirs)
def lookahead(fl, i):
    d = i
    while fl[d] != "};\n":
        d += 1
    return d + 1

nb = 0
la = 0
for i, line in enumerate(fl):
    if "const BehaviorScript" in line:
        tokens = line.split()
        bname = tokens[2].replace("[]","")
        nb += 1
        # if to_snake_case(bname.replace("bhv","")) in actdirs:
        la = lookahead(fl, i)
        bhvTable[bname] = fl[i:la]
        # else:?
        #     print(line[:-1])
    elif i < la:
        continue
    # else:
    #     print(line[:-1])


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
        if "extern const struct Animation *const" in l:
            tokens = l.split()
            hTable[h].append(tokens[5].split("[]")[0])

# print(hTable)
for b in bhvTable:
    nm = to_snake_case(b.replace("bhv",""))
    with open(f"data/behaviors/{nm}.c", "w+") as f:
        bhvData = ''.join([j for j in bhvTable[b]])
        f.write("#define OBJECT_FIELDS_INDEX_DIRECTLY\n")
        f.write('#include "sm64.h"\n')
        f.write('#include "types.h"\n')
        f.write("\n")
        if "SPAWN_CHILD" in bhvData or "SPAWN_OBJ" in bhvData:
            f.write('#include "behavior_data.h"\n')
        f.write('#include "object_constants.h"\n')
        f.write('#include "game/object_helpers.h"\n')
        f.write('#include "game/object_list_processor.h"\n')
        f.write('#include "game/behavior_actions.h"\n')
        if "SET_INTERACT_TYPE" in bhvData or "oInteractType" in bhvData or "oInteractionSubtype" in bhvData:
            f.write('#include "game/interaction.h"\n')
        if "load_object_collision_model" in bhvData or "load_object_static_model" in bhvData:
            f.write('#include "engine/surface_load.h"\n')
        if "beh_yellow_background_menu" in bhvData:
            f.write('#include "menu/file_select.h"\n')
        f.write("\n");
        for h in hTable:
            for k in hTable[h]:
                if k in bhvData:
                    f.write(f'#include "{h}"\n')
        f.write('#include "behavior_commands.h"\n')
        f.write('\n')
        f.write(bhvData)

