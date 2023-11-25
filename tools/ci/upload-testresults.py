import os
import sys
import glob

env = os.environ.copy()

if env["APPVEYOR_JOB_ID"] is None:
    print("Not running in AppVeyor, exiting", flush=True)
    sys.exit(1)

# Get all files named "test*.xml"
test_files = glob.glob("test*.xml", root_dir="build")
print("Test files:", test_files)
