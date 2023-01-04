# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import os
import sys
import subprocess
import shutil

doxygen_version = "1.9.5"


def is_doxygen_installed():
    try:
        installed_version = subprocess.check_output(
            ["C:\\PROGRA~1\\doxygen\\bin\\doxygen.exe", "--version"])
        print("doxygen --version\n%s" % (installed_version))
        installed_version = installed_version.decode().replace("\n", "")
        if installed_version.startswith(doxygen_version):
            return True
        else:
            print("Expected version %s, found version information %s" %
                  doxygen_version, installed_version)
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
