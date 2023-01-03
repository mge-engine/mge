# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

import os
import sys
import subprocess
import shutil

vcpkg_path = sys.argv[1]


def update_vcpkg():
    print("Pulling upstream vcpkg", flush=True)
    subprocess.run(["git", "fetch", "upstream"], cwd=vcpkg_path)
    print("Merge upstream vcpkg", flush=True)
    subprocess.run(["git", "merge", "upstream/master"], cwd=vcpkg_path)
    print("Updating installed packages")
    subprocess.run([os.path.join(vcpkg_path, "vcpkg.exe"),
                    "upgrade", "--no-dry-run"], cwd=vcpkg_path)


try:
    print(f"Vcpkg path is {vcpkg_path}")
    if not os.path.exists(os.path.join(vcpkg_path, "vcpkg.exe")):
        raise RuntimeError(
            "Vcpkg does not exist in specified installation path")

    if not os.path.exists("./tools/ci/vcpkg.py"):
        raise RuntimeError(
            "Script is not called from toplevel build directory")
    update_vcpkg()
except:
    print(sys.exc_info(), flush=True)
    sys.exit(1)

sys.exit(0)
