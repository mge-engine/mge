import sys
import os
import subprocess

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
    try:
        env = os.environ.copy()
        if "update gh-pages" in env["APPVEYOR_REPO_COMMIT_MESSAGE"]:
            branch = "main"  # treat this as dev version
            print("Updating gh-pages due to special commit message", flush=True)
            return True
    except:
        pass
    return False


def upload():
    print("Cloning gh-pages branch", flush=True)
    subprocess.run(["git", "clone", "-q", "--branch=gh-pages",
                    "https://github.com/mge-engine/mge.git", "gh-pages"], shell=True)
    subprocess.run(["git", "rm", "-rf", branch +
                    "/manual-html"], cwd="gh-pages")
    with open("gh-pages/" + branch + "/manual-html/.nojekyll") as nojekyll:
        nojekyll.write("no jekyll here please")
    subprocess.run(
        ["git", "add", branch + "/manual-html/.nojekyll"], cwd="gh-pages")
    subprocess.run(["git", "commit", "-m", message], cwd="gh-pages")


try:
    if upload_enabled():
        upload()
    else:
        print("No upload to gh-pages from this build")
except:
    print("Exception %s" % str(sys.exc_info()))
    sys.exit(1)

sys.exit(0)
