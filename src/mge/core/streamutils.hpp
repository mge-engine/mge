// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/config.hpp"
#include <iostream>
#include <cstdlib>

#ifdef MGE_COMPILER_MSVC
#  include <malloc.h>
#endif

namespace mge {
    namespace {
        struct line_printer
        {
            uint32_t length;
            char     c;
        };

        inline std::ostream& operator <<(std::ostream& os, const line_printer& p)
        {
            uint32_t remaining = p.length;
            const uint32_t partlen = 128;
            if(remaining > partlen) {
                char part[partlen + 1];
                memset(part, p.c, partlen);
                part[partlen]='\0';
                while (remaining > partlen) {
                    os << part;
                    remaining -= partlen;
                }
            }
            char *mem = (char *)alloca(remaining + 1);
            memset(mem, p.c, remaining);
            mem[remaining]='\0';
            return os << mem;
        }
    }

    /**
     * @brief Create a line printer.
     *
     * @param length length of line
     * @param c       character to compose the line of
     * @return line printer that prints a line of designated
     *              length
     */
    inline line_printer line(uint32_t length, char c='-')
    {
        line_printer p = { length, c};
        return p;
    }
}