# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import os
import sys
import subprocess
import shutil

doxygen_min_version = "1.9.6"

def is_same_or_newer_version(version, min_version):
    version_parts = version.split(".")
    min_version_parts = min_version.split(".")
    for i in range(0, len(min_version_parts)):
        version_parts[i] = version_parts[i].split()[0]  # Remove everything after 1st whitespace
        if int(version_parts[i]) < int(min_version_parts[i]):
            return False
    return True

def is_doxygen_installed():
    try:
        installed_version = subprocess.check_output(
            ["C:\\PROGRA~1\\doxygen\\bin\\doxygen.exe", "--version"])
        print("doxygen --version\n%s" % (installed_version), flush=True)
        installed_version = installed_version.decode().replace("\n", "")

        if is_same_or_newer_version(installed_version, doxygen_min_version):
            return True
        else:
            print("Expected minimal version %s, found version information %s" %
                  (doxygen_min_version, installed_version))
            return False
    except:
        print("Cannot determine doxygen version %s" %
              str(sys.exc_info()), flush=True)
        return False


def install_doxygen():
    print("Installing doxygen", flush=True)
    subprocess.run(["choco", "install", "-y", "doxygen.install"])


try:
    if not is_doxygen_installed():
        install_doxygen()
        if not is_doxygen_installed():
            raise Exception(
                "No doxygen of expected version found after installation")
except:
    print("Exception %s" % str(sys.exc_info()), flush=True)
    sys.exit(1)

sys.exit(0)
