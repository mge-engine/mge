import os

def process_make_file(filename):
    f = open(filename)
    lines = f.readlines()
    f.close()
    curated_lines=[]
    curated_lines.append("# Copyright (c) 2017 by Alexander Schroeder\n")
    headercomment = True
    for l in lines:
        if headercomment:
            if not l.startswith("#"):
                curated_lines.append(l)
                headercomment = False
        else:
            curated_lines.append(l)
    f = open(filename, "w")
    for l in curated_lines:
        f.write(l)
    f.close()

def process_source_file(filename):
    f = open(filename)
    lines = f.readlines()
    f.close()
    curated_lines=[]
    curated_lines.append("/*\n")
    curated_lines.append(" * Copyright (c) 2017 by Alexander Schroeder\n")
    curated_lines.append(" */\n")
    if len(lines) > 0:
        if "/*" in lines[0]:
            headercomment = True
            for l in lines:
                if headercomment:
                    if '*/' in l:
                        headercomment = False
                else:
                    curated_lines.append(l)
        else:
            for l in lines:
                curated_lines.append(l)
        f = open(filename, "w")
        for l in curated_lines:
            f.write(l)
        f.close()


def process_file(filename):
    if "CMakeLists.txt" in filename or filename.endswith(".cmake"):
        process_make_file(filename)
    elif filename.endswith(".cpp") or filename.endswith(".hpp"):
        process_source_file(filename)

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
            currentFile = "/".join(filelist)
            print("%s" % currentFile)
            process_file(currentFile)

