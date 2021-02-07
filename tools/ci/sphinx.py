import os
import sys
import subprocess
import shutil

sphinx_version = "1"
sphinx_dir = "sphinx"


def is_sphinx_installed():
    try:
        with open("sphinx/build-version") as f:
            installed_version = f.read().replace("\n", "")
            print("Found installed sphinx version %s" %
                  (installed_version), flush=True)
            if installed_version == sphinx_version:
                print("Required version matches installed version", flush=True)
                return True
            else:
                print("Required version is %s, need to install sphinx" %
                      sphinx_version, flush=True)
                return False
    except:
        return False


def install_sphinx():
    if os.path.exists(sphinx_dir):
        print("Removing previous installation")
        shutil.rmtree(sphinx_dir)
    print("Installing virtualenv", flush=True)
    subprocess.run(["python", "-m", "pip", "install",
                    "virtualenv"], shell=True)
    subprocess.run(["python", "-m", "virtualenv", sphinx_dir], shell=True)


try:
    if not is_sphinx_installed():
        install_sphinx()
except:
    print("Exception %s" % str(sys.exc_info()), flush=True)
    sys.exit(1)

sys.exit(1)
