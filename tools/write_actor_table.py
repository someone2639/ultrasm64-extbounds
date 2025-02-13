import sys, os, glob

build_dir = sys.argv[1]
actorfiles = [i for i in glob.glob("actors/**/data.c") if "vanilla_actors" not in i]


# TODO: compress rodata
actorentry = """BEGIN_SEG(%s, 0x04000000) SUBALIGN(16) {
    KEEP(BUILD_DIR/%s(.rodata*));
    . += ALIGN(16);
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


