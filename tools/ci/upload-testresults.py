import os
import sys

current_directory = os.getcwd()
print("Current directory:", current_directory)

env = os.environ.copy()

if env["APPVEYOUR_JOB_ID"] is None:
    print("Not running in AppVeyor, exiting", flush=True)
    sys.exit(1)

