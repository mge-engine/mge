import os
import sys
import subprocess
import shutil

boutput = subprocess.check_output(["vcpkg", "list"])

output = boutput.decode("UTF-8")

for line in output.splitlines():
    (comp, vers, *rest) = line.split(" ")
    subprocess.run(["vcpkg", "remove", "--recurse", comp])
