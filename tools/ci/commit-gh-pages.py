# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import sys
import os
import subprocess
import shutil
import os

def run_command(command, shell=False, cwd=None):
    env = os.environ.copy()
    process = subprocess.Popen(
        args=command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
        shell=shell,
        cwd=cwd,
        env=env
    )
    
    for line in process.stdout:
        print(line, end='', flush=True)

    process.stdout.close()
    return_code = process.wait()

    if return_code:
        raise subprocess.CalledProcessError(return_code, command)

upload_branches = ["main"]
branch = ""
event = ""
try:
    branch = os.environ["CURRENT_BRANCH"]
except:
    pass
try:
    event = os.environ["CURRENT_EVENT"]
except:
    pass
message = "Update gh-pages from generated documentation"

def upload_enabled():
    try:
        env = os.environ.copy()
        if event != "push":
            print("Commit is not a push, not uploading", flush=True)
            return False
        if branch in upload_branches:
            print("Branch is %s, upload enabled" %
                  (env["CURRENT_BRANCH"]), flush=True)
            return True
    except:
        pass
    return False

def upload_enabled_sys_argv():
    try:
        if sys.argv[1] == '-f':
            branch = "main"
            print("Updating gh-pages due to force flag", flush=True)
            return True
    except:
        pass
    return False


def copy2_verbose(src, dst):
    print('Copying {0}'.format(src), flush=True)
    shutil.copy2(src, dst)

def upload(branch):
    print("Switching to 'build' directory", flush=True)
    os.chdir("build")  # Change current directory to "build"
    print("Remove old files", flush=True)
    run_command(["git", "rm", "-rf", branch +
                    "/manual-html"], cwd="gh-pages" ,shell=True)
    run_command(["dir", "..\\docsrc\\manual\\manual-html"],
                cwd="gh-pages", shell=True)
    shutil.copytree("docsrc/manual/manual-html",
                    "gh-pages/" + branch + "/manual-html", copy_function=copy2_verbose)
    print("Adding files to git", flush=True)
    run_command(
        ["git", "add", branch + "/manual-html"], cwd="gh-pages")
    print("Commit git changes", flush=True)
    run_command(["git", "commit", "-m", message], cwd="gh-pages")

try:
    if upload_enabled():
        upload(branch)
    elif upload_enabled_sys_argv():
        upload("main")
    else:
        print("No upload to gh-pages from this build")
except:
    print("Exception %s" % str(sys.exc_info()))
    sys.exit(1)

sys.exit(0)
