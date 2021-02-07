import sys
import os
import subprocess
import shutil

upload_branches = ["main"]
branch = ""
try:
    branch = os.environ["APPVEYOR_REPO_BRANCH"]
except:
    pass
message = "Update gh-pages from generated documentation"


def upload_enabled():
    try:
        env = os.environ.copy()
        if branch in upload_branches:
            print("Branch is %s, upload enabled" %
                  (env["APPVEYOR_REPO_BRANCH"]), flush=True)
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


def upload(branch):
    print("Cloning gh-pages branch", flush=True)
    subprocess.run(["git", "clone", "-q", "--branch=gh-pages",
                    "https://github.com/mge-engine/mge.git", "gh-pages"], shell=True)
    print("Remove old files", flush=True)
    subprocess.run(["git", "rm", "-rf", branch +
                    "/manual-html"], cwd="gh-pages")
    nojekyllfilename = "gh-pages/" + branch + "/manual-html/.nojekyll"
    print("Touching nojekyll file", flush=True)
    with open("gh-pages/" + branch + "/manual-html/.nojekyll", "w") as nojekyll:
        nojekyll.write("no jekyll here please")
    print("Adding .nojekyll file to git", flush=True)
    subprocess.run(
        ["git", "add", branch + "/manual-html/.nojekyll"], cwd="gh-pages")
    print("Commit git changes (nojekyll file)", flush=True)
    subprocess.run(["git", "commit", "-m", message], cwd="gh-pages")
    print("Copy generated documentation", flush=True)
    shutil.copytree("docsrc/manual/manual-html", branch, dirs_exist_ok=True)
    print("Adding to commit", flush=True)
    subprocess.run(
        ["git", "add", branch + "/manual-html"], cwd="gh-pages")
    print("Amending commit", flush=True)
    subprocess.run(["git", "commit", "--amend", "-C", "HEAD"], cwd="gh-pages")


try:
    if upload_enabled():
        upload(branch)
    elif upload_enabled_special_commit():
        upload("main")

    else:
        print("No upload to gh-pages from this build")
except:
    print("Exception %s" % str(sys.exc_info()))
    sys.exit(1)

sys.exit(0)
