import os
import sys
import glob
import requests

env = os.environ.copy()

if env["APPVEYOR_JOB_ID"] is None:
    print("Not running in AppVeyor, exiting", flush=True)
    sys.exit(1)

# Get all files named "test*.xml"
test_files = glob.glob("build/test*.xml")
print("Test files:", test_files)

# URL to upload the test files
upload_url = "https://ci.appveyor.com/api/testresults/junit/" + env["APPVEYOR_JOB_ID"]


for file in test_files:
    with open(file, "rb") as f:
        response = requests.post(upload_url, files={"file": f})
        if response.status_code == 200:
            print(f"Uploaded {file} successfully")
        else:
            print(f"Failed to upload {file}. Status code: {response.status_code}")
