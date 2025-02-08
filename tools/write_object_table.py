import sys, os, subprocess

def usage():
    print(f"{sys.argv[0]} <build dir> [list of object behavior files]")

obj_ld_file = sys.argv[1]+"/objects.ld"
obj_defs_file = sys.argv[1]+"/object_defs.ld"
obj_behaviors = sys.argv[2:]

packet = """BEGIN_SEG(%s, 0) {
    KEEP(%s(.rodata*));
}
END_SEG(%s)
"""


bhvTable = {}
sizeTable = {}

def ToCamelCase(s):
    ret = [i.capitalize() for i in s.split("_")]
    for i in range(len(ret)):
        if ret[i].upper() in ["TTC", "DDD", "SL"]:
            ret[i] = ret[i].upper()
        elif ret[i].upper() in ["HMC", "CCM","LLL"]:
            ret[i] = ret[i].lower().capitalize()
    return "".join(ret)

with open(obj_ld_file, "w+") as f:
    for i, nm in enumerate(obj_behaviors):
        name = nm.split("/")[-1].replace(".o","")
        if name[0].isdigit():
            name = "_" + name
        bhvTable[i] = name
        name += "_bhv"

        f.write(packet % (name, nm, name))

with open(obj_defs_file, "w+") as f:
    f.write("BEGIN_SEG(object_table, __romPos) SUBALIGN(4) {\n")
    for k in bhvTable:
        f.write(f"LONG(_{bhvTable[k]}_bhvSegmentRomStart); LONG(SIZEOF(.{bhvTable[k]}_bhv));\n")
    f.write("}\nEND_SEG(object_table)\n\n")
    for k in bhvTable:
        f.write(f"bhv{ToCamelCase(bhvTable[k])} = {k};\n")
