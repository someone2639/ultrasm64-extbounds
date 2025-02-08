import sys, os, glob

build_dir = sys.argv[1]
actorfiles = glob.glob("actors/**/data.c")


# TODO: compress rodata
actorentry = """BEGIN_SEG(%s, 0x04000000) {
    KEEP(BUILD_DIR/%s(.rodata*));
}
END_SEG(%s)
"""

with open(build_dir+"/actor_table.ld", "w+") as f:
    for i in actorfiles:
        ofile = i.replace(".c", ".o")
        segname = f"{os.path.dirname(ofile).split("/")[-1]}_actor_data"
        f.write(actorentry % (
            segname,
            ofile,
            segname
        ))


