
import os
import sys
import subprocess

version = 1
vcpkg_githash = "70422b96724101d67e88923539cf2ea2c880c39a"
vcpkg_packages = [
    "gtest", "boost-preprocessor", "boost-filesystem",
    "boost-circular-buffer", "boost-lexical-cast", "boost-property-tree"
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


if os.path.exists("vcpkg") and os.path.isdir("vcpkg"):
    print("vcpkg path exists", flush=True)
else:
    print("vcpkg path not found, performing complete install", flush=True)
    install_vcpkg()
    sys.exit(1)


sys.exit(1)

#   - cmd: git clone https://github.com/mge-engine/vcpkg.git
#  - cmd: cd vcpkg
#  - cmd: git checkout 70422b96724101d67e88923539cf2ea2c880c39a
#  - cmd: .\bootstrap-vcpkg.bat -disableMetrics
#  - cmd: .\vcpkg.exe install --triplet x64-windows gtest
#  - cmd: .\vcpkg.exe install --triplet x64-windows boost-preprocessor boost-filesystem boost-circular-buffer
#  - cmd: .\vcpkg.exe install --triplet x64-windows boost-lexical-cast boost-property-tree
#  - cmd: cd ..
# advice.detachedHead
