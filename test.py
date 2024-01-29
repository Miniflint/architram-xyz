with open ("./xyz_files/SWISSALTI3D_2_XYZ_CHLV95_LN02_2535_1154.xyz", "r") as f:
    readed = f.read()

splitted = readed.split()

splitted = splitted[3:]
floats = [float(x) for x in splitted] 