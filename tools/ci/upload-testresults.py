import os
import sys
import glob

current_directory = os.getcwd()
print("Current directory:", current_directory)

env = os.environ.copy()

if env["APPVEYOR_JOB_ID"] is None:
    print("Not running in AppVeyor, exiting", flush=True)
    sys.exit(1)

# Get all files named "test*.xml"
test_files = glob.glob("test*.xml")
print("Test files:", test_files)
