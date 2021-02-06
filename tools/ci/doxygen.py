import os
import sys
import subprocess
import shutil

doxygen_version = "1.9.1"


def is_doxygen_installed():
    try:
        installed_version = subprocess.check_output(
            [".\\doxygen.exe", "--version"], cwd="C:\\PROGRA~1\\doxygen\\bin")
        print("Doxygen %s found" % installed_version, flush=True)
        if installed_version.startswith(doxygen_version):
            return True
        else:
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
except:
    print("Exception %s" % str(sys.exc_info()), flush=True)
    sys.exit(1)

sys.exit(0)
