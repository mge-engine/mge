# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
import os
import sys
import subprocess
import shutil

sphinx_version = "3"
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
    subprocess.run(["python", "-m", "virtualenv", sphinx_dir], shell=True,)
    # manually activate
    sphinx_env = os.environ.copy()
    try:
        del(sphinx_env["PYTHONPATH"])
    except:
        pass
    sphinx_env["PATH"] = os.path.abspath(
        sphinx_dir + "/Scripts") + ";" + sphinx_env["PATH"]
    sphinx_env["VIRTUAL_ENV"] = os.path.abspath(sphinx_dir)
    subprocess.run(["python", "-m", "pip", "install",
                    "--upgrade", "pip"], shell=True, env=sphinx_env)
    subprocess.run(["python", "-m", "pip", "install",
                    "sphinx"], shell=True, env=sphinx_env)
    subprocess.run(["python", "-m", "pip", "install",
                    "breathe"], shell=True, env=sphinx_env)
    subprocess.run(["python", "-m", "pip", "install",
                    "sphinx_rtd_theme"], shell=True, env=sphinx_env)
    subprocess.run(["python", "-m", "pip", "install",
                    "furo"], shell=True, env=sphinx_env)
    version_file = open(sphinx_dir + "/build-version", "w")
    version_file.write(sphinx_version)
    version_file.close()


try:
    if not is_sphinx_installed():
        install_sphinx()
except:
    print("Exception %s" % str(sys.exc_info()), flush=True)
    sys.exit(1)

sys.exit(0)
