# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
import os
import sys
import copy

dry_run = False

def write_file(filename, lines):
    print "updating %s" % filename
    if dry_run:
        return
    f = open(filename, "w")
    for l in lines:
        f.write(l)
    f.close()


def process_make_file(filename):
    f = open(filename)
    lines = f.readlines()
    f.close()
    curated_lines=[]
    curated_lines.append("# mge - Modern Game Engine\n")
    curated_lines.append("# Copyright (c) 2018 by Alexander Schroeder\n")
    curated_lines.append("# All rights reserved.\n")
    headercomment = True
    for l in lines:
        if headercomment:
            if not l.startswith("#"):
                curated_lines.append(l)
                headercomment = False
        else:
            curated_lines.append(l)
    write_file(filename, curated_lines)

def process_cpp_source_file(filename):
    f = open(filename)
    lines = f.readlines()
    f.close()
    curated_lines=[]
    headercomment = True
    headercomment_found = False
    for l in lines:
        if headercomment:
            if "#pragma once" in l:
                curated_lines.append(l)
            elif not l.startswith("//"):
                curated_lines.append("// mge - Modern Game Engine\n")
                curated_lines.append("// Copyright (c) 2018 by Alexander Schroeder\n")
                curated_lines.append("// All rights reserved.\n")
                curated_lines.append(l)
                headercomment = False
                headercomment_found = True
        else:
            curated_lines.append(l)

    if not headercomment_found == 0:
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

    write_file(filename, curated_lines)

def process_c_source_file(filename):
    f = open(filename)
    lines = f.readlines()
    f.close()
    curated_lines=[]
    curated_lines.append("/* mge - Modern Game Engine\n")
    curated_lines.append(" * Copyright (c) 2018 by Alexander Schroeder\n")
    curated_lines.append(" * All rights reserved.\n")
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

    write_file(filename, curated_lines)


def process_file(filename):
    if "CMakeLists.txt" in filename or filename.endswith(".cmake"):
        process_make_file(filename)
    elif filename.endswith(".py"):
        process_make_file(filename)
    elif filename.endswith(".cpp") or filename.endswith(".hpp"):
        process_cpp_source_file(filename)
    elif filename.endswith(".c") or filename.endswith(".h"):
        process_c_source_file(filename)

def blacklisted(path):
    if path[0].startswith('.') and len(path)>1:
        return True
    elif path[0].startswith("build"):
        return True
    else:
        return False


args = copy.copy(sys.argv[1:])
if "-n" in sys.argv:
    dry_run = True
    args.remove('-n')

if len(args) == 0:
    args.append('.')

for rootdir in args:
    print "Processing file/directory %s" % (rootdir)
    for root, dirs, files in os.walk(rootdir):
        path = root.split(os.sep)
        for f in files:
            filelist = []
            filelist.extend(path)
            filelist.append(f)
            if rootdir == '.':
                filelist = filelist[1:]
            if(not blacklisted(filelist)):
                currentFile = "/".join(filelist)
                print("processing %s" % currentFile)
                process_file(currentFile)

