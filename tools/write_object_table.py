import sys, os, subprocess

def usage():
    print(f"{sys.argv[0]} <build dir> [list of object behavior files]")

obj_ld_file = sys.argv[1]+"/objects.ld"
obj_defs_file = sys.argv[1]+"/object_defs.ld"
obj_s_file = sys.argv[1]+"/object_table.s"
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
        p = subprocess.Popen(["readelf", "-S", nm], stdout=subprocess.PIPE)
        name = nm.split("/")[-1].replace(".o","")
        if name[0].isdigit():
            name = "_" + name
        bhvTable[i] = name
        name += "_bhv"
        sizes = p.communicate()[0].decode('ascii')
        for l in sizes.split("\n"):
            if ".rodata.bhv" in l:
                sizeTable[i] = int(l.split()[6], 16)

        f.write(packet % (name, nm, name))

with open(obj_defs_file, "w+") as f:
    f.write("""BEGIN_SEG(object_table, __romPos) {
    KEEP(BUILD_DIR/object_table.o(.rodata));
}
END_SEG(object_table)
""")
    for k in bhvTable:
        f.write(f"bhv{ToCamelCase(bhvTable[k])} = {k};\n")

# TODO: write this in linker script format instead of using 500 readelf invocs
with open(obj_s_file, "w+") as f:
    f.write('#include "macros.inc"\n')
    f.write(".section .rodata\n")
    f.write("glabel gObjectTable\n")
    for k in bhvTable:
        f.write(f".word _{bhvTable[k]}_bhvSegmentRomStart, {sizeTable[k]}\n")
    f.write("glabel gObjectTableEnd\n")
