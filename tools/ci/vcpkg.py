
import os
import sys
import subprocess
import shutil

# parameters
# -- version            increment each time config changes
# -- vcpkg_githash      git hash of vcpkg to check out
# -- vcpkg_packages     array of package names to install
version = "1"
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
    install_command = ["vcpkg.exe", "install", "--triplet", "x64-windows"]
    install_command.extend(vcpkg_packages)
    print("Installing following packages:", flush=True)
    for p in vcpkg_packages:
        print("    %s" % p, flush=True)
    subprocess.run(install_command, cwd="vcpkg")
    version_file = open("vcpkg/build-version", "w")
    version_file.write(version)
    version_file.close()


def is_vcpkg_installed():
    try:
        with open("vcpkg/build-version") as f:
            installed_version = f.read().replace("\n", "")
            print("found installed vcpkg version %s" %
                  (installed_version), flush=True)
            if installed_version == version:
                print("required version matches installed version", flush=True)
                return True
            else:
                print("required version is %s, need to install vcpkg" %
                      version, flush=True)
                return False
    except:
        return False


try:
    if not is_vcpkg_installed():
        if os.path.exists("vcpkg") and os.path.isdir("vcpkg"):
            print("vcpkg path exists, renoving it", flush=True)
            shutil.rmtree("vcpkg")
        print("performing complete install", flush=True)
        install_vcpkg()
except:
    print(sys.exc_info, flush=True)
    sys.exit(1)

sys.exit(0)


#   - cmd: git clone https://github.com/mge-engine/vcpkg.git
#  - cmd: cd vcpkg
#  - cmd: git checkout 70422b96724101d67e88923539cf2ea2c880c39a
#  - cmd: .\bootstrap-vcpkg.bat -disableMetrics
#  - cmd: .\vcpkg.exe install --triplet x64-windows gtest
#  - cmd: .\vcpkg.exe install --triplet x64-windows boost-preprocessor boost-filesystem boost-circular-buffer
#  - cmd: .\vcpkg.exe install --triplet x64-windows boost-lexical-cast boost-property-tree
#  - cmd: cd ..
# advice.detachedHead
