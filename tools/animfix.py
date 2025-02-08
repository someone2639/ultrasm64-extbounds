import glob

files = glob.glob("actors/**/anims/anim_*.inc.c")
print(files)

for i in files:
    fl = []
    with open(i) as f:
        fl = f.readlines()
    with open(i, "w+") as f:
        for line in fl:
            if "const u16" in line and "animindex" in line:
                f.write(line.replace("const u16", "const AnimIndex"))
            elif "const s16" in line and "animvalue" in line:
                f.write(line.replace("const s16", "const AnimValue"))
            else:
                f.write(line)
