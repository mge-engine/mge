# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

import os
import sys
import subprocess
import shutil

# parameters
# -- version            increment each time config changes
# -- vcpkg_githash      git hash of vcpkg to check out
# -- vcpkg_packages     array of package names to install
version = "31"
vcpkg_githash = "0ba60bfef5dea4cb2599daa7ad8364e309835a68"
vcpkg_packages = [
    "gtest", "boost-preprocessor", "boost-filesystem",
    "boost-circular-buffer", "boost-lexical-cast", "boost-property-tree",
    "boost-spirit", "boost-locale",
    "vulkan", "glm", "lua", "python3", "linenoise-ng"
]

def install_vcpkg():
    print("Cloning vcpkg git mirror", flush=True)
    subprocess.run(["git", "clone", "https://github.com/mge-engine/vcpkg.git"])
    subprocess.run(
        ["git", "config", "advice.detachedHead", "false"], cwd="vcpkg")
    print("Checking out configured version %s" % (vcpkg_githash), flush=True)
    subprocess.run(["git", "checkout", vcpkg_githash], cwd="vcpkg")
    print("Bootstrapping vcpkg", flush=True)
    subprocess.run([".\\bootstrap-vcpkg.bat", "-disableMetrics"],
                   cwd="vcpkg", shell=True)
    # use x64-windows-release to force dynamic runtime
    install_command = ["vcpkg.exe", "install",
                       "--triplet", "x64-windows"]
    install_command.extend(vcpkg_packages)
    print("Installing following packages:", flush=True)
    for p in vcpkg_packages:
        print("    %s" % p, flush=True)
    subprocess.run(install_command, cwd="vcpkg", shell=True)
    print("Removing temporary vcpkg files", flush=True)
    shutil.rmtree("vcpkg/downloads")
    print("Writing version stamp", flush=True)
    version_file = open("vcpkg/build-version", "w")
    version_file.write(version)
    version_file.close()


def is_vcpkg_installed():
    try:
        with open("vcpkg/build-version") as f:
            installed_version = f.read().replace("\n", "")
            print("Found installed vcpkg version %s" %
                  (installed_version), flush=True)
            if installed_version == version:
                print("Required version matches installed version", flush=True)
                return True
            else:
                print("Required version is %s, need to install vcpkg" %
                      version, flush=True)
                return False
    except:
        return False


try:
    if not is_vcpkg_installed():
        if os.path.exists("vcpkg") and os.path.isdir("vcpkg"):
            print("Vcpkg path exists, removing it", flush=True)
            shutil.rmtree("vcpkg")
        print("Performing complete install", flush=True)
        install_vcpkg()
except:
    print(sys.exc_info, flush=True)
    sys.exit(1)

sys.exit(0)
