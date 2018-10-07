# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
import os
import sys
import glob

files = glob.glob(sys.argv[1] + '*')

for f in files:
    try:
        os.remove(f)
    except:
        pass






