import glob, os

lfolders = [i for i in glob.glob("levels/*") if os.path.isdir(i)]

llist = []

for f in lfolders:
    lname = f.split("/")[-1]
    llist.append(lname)
    print(f"const LevelScript script_load_{lname}_models[] = {{")
    sb = ""
    with open(f+"/script.c") as script:
        sb = script.read()

    with open(f+"/script.c", "w+") as sf:
        for line in sb.split("\n"):
            if "LOAD_MODEL_FROM_GEO" in line:
                print(f"{line.replace("LEVEL_GEOMETRY", f"{lname}_LEVEL_GEOMETRY")}")
            elif "JUMP_LINK" in line and "script_func_global" in line:
                pass
            else:
                sf.write(f"{line}\n")


    print("    RETURN(),")
    print("};")
    print()

