# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
import sys
import os
import subprocess
import shutil
import os

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
        if pull_request_number != "":
            print("Commit is a pull request, not uploading", flush=True)
            return False
        if branch in upload_branches:
            print("Branch is %s, upload enabled" %
                  (env["CURRENT_BRANCH"]), flush=True)
            return True
    except:
        pass
    return False


def upload_enabled_special_commit():
    try:
        env = os.environ.copy()
        if "update gh-pages" in env["APPVEYOR_REPO_COMMIT_MESSAGE"]:
            branch = "main"  # treat this as dev version
            print("Updating gh-pages due to special commit message", flush=True)
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
    if os.path.exists("gh-pages"):
        print("Remove old gh-pages directory", flush=True)
        subprocess.run(["rd", "/s", "/q", "gh-pages"], shell=True)
    print("Cloning gh-pages branch", flush=True)
    subprocess.run(["git", "clone", "--branch=gh-pages",
                    "https://github.com/mge-engine/mge.git", "gh-pages"], shell=True)
    print("Remove old files", flush=True)
    subprocess.run(["git", "rm", "-rf", branch +
                    "/manual-html"], cwd="gh-pages" ,shell=True)
    subprocess.run(["dir", "..\\docsrc\\manual\\manual-html"],
                cwd="gh-pages", shell=True)
    shutil.copytree("docsrc/manual/manual-html",
                    "gh-pages/" + branch + "/manual-html", copy_function=copy2_verbose)
    print("Adding files to git", flush=True)
    subprocess.run(
        ["git", "add", branch + "/manual-html"], cwd="gh-pages")
    print("Commit git changes", flush=True)
    subprocess.run(["git", "commit", "-m", message], cwd="gh-pages")
    subprocess.run(["git", "push", "origin"], cwd="gh-pages")

try:
    if upload_enabled():
        upload(branch)
    elif upload_enabled_special_commit() or upload_enabled_sys_argv():
        upload("main")
    else:
        print("No upload to gh-pages from this build")
except:
    print("Exception %s" % str(sys.exc_info()))
    sys.exit(1)

sys.exit(0)
