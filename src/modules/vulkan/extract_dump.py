import sys
import xml.etree.ElementTree as ET

sourcefile = sys.argv[1]
targetFile = sys.argv[2]


fileheader = """// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
# pragma once
# include "mge/core/details.hpp"
# include "mge/core/gist.hpp"
# include "vulkan.hpp"
# include <iostream>

namespace {



}
"""

tree = ET.parse(sourcefile)
root = tree.getroot()


class EnumValue:
    def __init__(self, name, value):
        self.name = name
        self.value = value


class PureEnum:
    def __init__(self, element):
        self.name = element.attrib['name']
        self.values = []
        for el in element.iter("enum"):
            if el.get("alias") is None:
                self.values.append(EnumValue(el.get("name"), el.get("value")))

    def emit(self, f):
        print("""inline std::ostream& operator <<(std::ostream& os, const %s& v)
{
    switch(v) {""" % (self.name), file=f)
        for v in self.values:
            print("""    case %s: return os << "%s"; """ %
                  (v.name, v.name), file=f)
        print("""    default: return os << "UNKNOWN(" << static_cast<int>(v) << ")";
    }
}""", file=f)


all_enums = []

for el in root.iter('enums'):
    if (el.get("type") == 'enum'):
        all_enums.append(PureEnum(el))

with open(targetFile, "w") as f:
    print(fileheader, file=f)
    for e in all_enums:
        e.emit(f)
