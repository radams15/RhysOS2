#!/usr/bin/env python3
from sys import argv
from os import sep, makedirs, path

INC = "include"
SRC = "src"


name = argv[1]

parts = name.split(sep)
dirs = parts[0:-1]

file_name = parts[-1]


inc_dir = path.join(INC, *dirs)
src_dir = path.join(SRC, *dirs)
print(inc_dir, src_dir)

if not path.exists(inc_dir):
	makedirs(inc_dir)
else:
	print("Inc Path Exists!")

if not path.exists(src_dir):
	makedirs(src_dir)
else:
	print("Src Path Exists!")


source = path.join(SRC, *dirs, f"{file_name}.c")
header = path.join(INC, *dirs, f"{file_name}.h")

if not path.exists(source) and not path.exists(header):
	with open(source, "w") as f:
		f.write("#include <{}>".format(path.join(*dirs, f"{file_name}.h")))

	with open(header, "w") as f:
		f.write("#ifndef RHYSOS_{}_H\n#define RHYSOS_{}_H\n\n\n\n#endif //RHYSOS_{}_H".format(file_name.upper(), file_name.upper(), file_name.upper()))
