# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
import sys
import re

print("=== EXTRACT VULKAN INSTANCE FUNCTIONS ===")
sourceFile = sys.argv[1]
targetFile = sys.argv[2]
print("-- Source header file to process: {0}".format(sourceFile))
print("-- Target header file to write: {0}".format(targetFile))
lines = []
with open(sourceFile, "r") as f:
    l = f.readline()
    while l:
        lines.append(l)
        l = f.readline()

print("-- Read {0} lines".format(len(lines)))
# these functions or defines are not looked up by 
# vkGetInstanceProcAddr
excludedFunctions = [
    "vkAllocationFunction",
    "vkReallocationFunction",
    "vkFreeFunction",
    "vkInternalAllocationNotification",
    "vkInternalFreeNotification",
    "vkVoidFunction",
    "vkGetInstanceProcAddr",
    "vkCreateDebugReportCallbackEXT",
    "vkDestroyDebugReportCallbackEXT",
]

def is_basic_instance_function(f):
    explicitInstanceFunctions = [
        "vkEnumerateInstanceExtensionProperties",
        "vkEnumerateInstanceLayerProperties",
        "vkCreateInstance",
    ]
    return f in explicitInstanceFunctions


def is_instance_function(f, remainder):
    if f == "vkGetDeviceProcAddr":
        return True
    prefixes = [
        "(VkInstance instance,",
        "(VkPhysicalDevice physicalDevice,"
    ]
    for p in prefixes:
        if remainder.startswith(p):
            return True
    return False

writeLines = []

for line in lines:
    # (VKAPI_PTR *PFN_XXXXX)
    m = re.search("\(VKAPI_PTR \*PFN_(\w+)\)(.*)", line)
    if m:
        functionName = m.group(1)
        remainder = m.group(2)
        #print("-- Found function {0}".format(functionName))
        if functionName in excludedFunctions:
            print("-- No loaded function: {0}".format(functionName))
        elif is_basic_instance_function(functionName):
            writeLines.append("BASIC_INSTANCE_FUNCTION({0})".format(functionName))
        elif is_instance_function(functionName, remainder):
            writeLines.append("INSTANCE_FUNCTION({0})".format(functionName))
            print("-- Instance function: {0}".format(functionName))
        else:
            writeLines.append("DEVICE_FUNCTION({0})".format(functionName))
            print("-- Device function: {0}{1}".format(functionName, remainder))

with open(targetFile, "w") as f:
    print("/* Generated file, do not edit */", file=f)
    for l in writeLines:
        print(l, file=f)