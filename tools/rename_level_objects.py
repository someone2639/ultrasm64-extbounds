

mods = [
"special_level_geo_03",
"special_level_geo_04",
"special_level_geo_05",
"special_level_geo_06",
"special_level_geo_07",
"special_level_geo_08",
"special_level_geo_09",
"special_level_geo_0A",
"special_level_geo_0B",
"special_level_geo_0C",
"special_level_geo_0D",
"special_level_geo_0E",
"special_level_geo_0F",
"special_level_geo_10",
"special_level_geo_11",
"special_level_geo_12",
"special_level_geo_13",
"special_level_geo_14",
"special_level_geo_15",
"special_level_geo_16",
]

import glob, os

level_folders = glob.glob("levels/**");
lf = [i for i in level_folders if os.path.isdir(i)]


for i in lf:
    lname = i.split("/")[-1]
    for m in mods:
        m_old = m
        m_new = m.replace("level", f"{lname}_level")
        os.system(f"git grep -rl {m_old} {i} | xargs sed -i 's|{m_old}|{m_new}|g'")
