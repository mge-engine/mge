# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
import sys
import re

#
#def foreign_function(f):
#    if 'Xlib' in f:
#        return True
#    if 'Mir' in f:
#        return True
#    if 'RandR' in f:
#        return True
#    if 'Wayland' in f:
#        return True
#    if 'Xcb' in f:
#        return True
#    if 'Android' in f:
#        return True
#    if 'NN' in f:
#        return True
#    if '2KHR' in f:
#        return True
#    if 'AMD' in f:
#        return True
#    if 'NV' in f:
#        return True
#    if '2EXT' in f:
#        return True
#    if f in ['vkTrimCommandPoolKHR',
#              'vkDebugReportCallbackEXT',
#              'vkCreateDebugReportCallbackEXT',
#              'vkDestroyDebugReportCallbackEXT',
#              'vkDebugReportMessageEXT',
#              'vkDebugMarkerSetObjectTagEXT',
#              'vkDebugMarkerSetObjectNameEXT',
#              'vkCmdDebugMarkerBeginEXT',
#              'vkCmdDebugMarkerEndEXT',
#              'vkCmdDebugMarkerInsertEXT',
#              'vkReleaseDisplayEXT',
#              'vkDisplayPowerControlEXT',
#              'vkRegisterDeviceEventEXT',
#              'vkRegisterDisplayEventEXT',
#              'vkGetSwapchainCounterEXT',
#              'vkCreateIOSSurfaceMVK',
#              'vkCreateMacOSSurfaceMVK',
#              ]:
#        return True
#    return False

#input_file_name = sys.argv[1]
#header_output_file_name = sys.argv[2]
#resolve_output_file_name = sys.argv[3]
cpp_file = sys.argv[1]

#fpexp = re.compile(".*\(VKAPI_PTR \*PFN_(\w+)\)")

#all_functions = []
#create_instance_found = False

#input_file = open(input_file_name, "r")
#for l in input_file:
#    m = fpexp.match(l)
#    if m is not None:
#        func = m.group(1)
#        if func == 'vkCreateInstance':
#            create_instance_found = True
#        if create_instance_found:
#            all_functions.append(func)

#output_file = open(header_output_file_name, "w")
#output_file.write("/* This file is generated, do not edit */\n")
#for f in all_functions:
#    if not foreign_function(f):
#        output_file.write("PFN_{0} {0};\n".format(f))
#output_file.close()

#output_file = open(resolve_output_file_name, "w")
#output_file.write("/* This file is generated, do not edit */\n")
#for f in all_functions:
#    if not foreign_function(f):
#        output_file.write("RESOLVE_FUNCTION({0});\n".format(f))
#output_file.close()#

output_file = open(cpp_file, "w")
output_file.write("/* This file is generated, do not edit */\n")
output_file.close()
