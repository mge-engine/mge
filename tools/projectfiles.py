import os
import sys
import re

def blacklisted(path):
    if path[0].startswith('.') and len(path)>1:
        return True
    elif path[0].startswith("build"):
        return True
    else:
        return False

for root, dirs, files in os.walk("."):
    path = root.split(os.sep)
    for f in files:
        filelist = []
        filelist.extend(path)
        filelist.append(f)
        filelist = filelist[1:]
        if(not blacklisted(filelist)):
            print("%s" % "/".join(filelist)) 
            
