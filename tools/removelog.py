import os
import sys
import glob

files = glob.glob(sys.argv[1] + '*')

for f in files:
    try:
        os.remove(f)
    except:
        pass






