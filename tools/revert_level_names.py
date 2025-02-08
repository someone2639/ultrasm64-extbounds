
models = [
    "MODEL_LEVEL_GEOMETRY_03",
    "MODEL_LEVEL_GEOMETRY_04",
    "MODEL_LEVEL_GEOMETRY_05",
    "MODEL_LEVEL_GEOMETRY_06",
    "MODEL_LEVEL_GEOMETRY_07",
    "MODEL_LEVEL_GEOMETRY_08",
    "MODEL_LEVEL_GEOMETRY_09",
    "MODEL_LEVEL_GEOMETRY_0A",
    "MODEL_LEVEL_GEOMETRY_0B",
    "MODEL_LEVEL_GEOMETRY_0C",
    "MODEL_LEVEL_GEOMETRY_0D",
    "MODEL_LEVEL_GEOMETRY_0E",
    "MODEL_LEVEL_GEOMETRY_0F",
    "MODEL_LEVEL_GEOMETRY_10",
    "MODEL_LEVEL_GEOMETRY_11",
    "MODEL_LEVEL_GEOMETRY_12",
    "MODEL_LEVEL_GEOMETRY_13",
    "MODEL_LEVEL_GEOMETRY_14",
    "MODEL_LEVEL_GEOMETRY_15",
    "MODEL_LEVEL_GEOMETRY_16",
    ]

import glob, os
levels = [i for i in glob.glob("levels/**") if os.path.isdir(i)]

for lf in levels:
    lname = lf.split("/")[-1]
    for m in models:
        m_old = m.replace("LEVEL", f"{lname}_LEVEL")
        m_new = m
        # print(f"git grep -rl {m_old} levels/ | xargs sed -i 's|{m_old}|{m_new}|g'")
        os.system(f"git grep -rl {m_old} levels/ | xargs sed -i 's|{m_old}|{m_new}|g'")

