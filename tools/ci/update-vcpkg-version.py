# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import sys
import os
import subprocess
import ast


def update_vcpkg_version(vcpkg_path):
    p = subprocess.Popen(
        args=['git', 'log', '-1', '--pretty=format:"%H"'], cwd=vcpkg_path, stdout=subprocess.PIPE)
    out, err = p.communicate()
    githash = out.decode()[1:-1]
    return githash


def replace_githash(githash, file):
    print("Replacing git hash in {}".format(file))
    content = open(file, 'r').read()
    code = ast.parse(content, filename=file)
    version = int(code.body[0].value.value)
    old_githash = code.body[1].value.value
    version_module = """
# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
version = "{version}"
vcpkg_githash = "{githash}"
"""
    print("File version is {}".format(version))
    print("Old git hash {}".format(old_githash))
    print("New git hash {}".format(githash))
    version = version + 1
    if githash != old_githash:
        with open(file, 'w') as f:
            f.write(version_module.format(version=version, githash=githash))
        return True
    else:
        return False


def submit_new_version():
    subprocess.run(args=["git", "add", "./tools/ci/vcpkg_version.py"])
    subprocess.run(args=["git", "commit", "-m", "Update vcpkg"])
    subprocess.run(args=["git", "push"])


vcpkg_path = sys.argv[1]

try:
    print(f"Vcpkg path is {vcpkg_path}")
    if not os.path.exists(os.path.join(vcpkg_path, "vcpkg.exe")):
        raise RuntimeError(
            "Vcpkg does not exist in specified installation path")

    if not os.path.exists("./tools/ci/vcpkg.py"):
        raise RuntimeError(
            "Script is not called from toplevel build directory")
    githash = update_vcpkg_version(vcpkg_path)
    if (replace_githash(githash, "./tools/ci/vcpkg_version.py")):
        submit_new_version()

except:
    print(sys.exc_info(), flush=True)
    sys.exit(1)

sys.exit(0)
