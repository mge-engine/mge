// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.

#include "mge/core/type_name.hpp"
#include "mge/core/stdexceptions.hpp"
#include <cstring>

#ifdef MGE_COMPILER_GNUC
extern "C" char *
__cxa_demangle(const char* __mangled_name, char* __output_buffer,
               size_t* __length, int* __status);
#endif


namespace mge {

    MGE_CORE_EXPORT std::string type_name(const std::type_info& ti)
    {
        std::string raw_name(ti.name());
#ifdef MGE_COMPILER_MSVC
        if(strncmp(raw_name.c_str(), "class ", strlen("class "))==0) {
            raw_name = std::string(raw_name, 6);
        } else if(strncmp(raw_name.c_str(), "struct ", strlen("struct "))==0) {
            raw_name = std::string(raw_name, 7);
        } else if(strncmp(raw_name.c_str(), "enum ", strlen("enum "))==0) {
            raw_name = std::string(raw_name, 5);
        }
        auto pos = raw_name.find("__ptr64");
        while (pos != std::string::npos) {
            raw_name.replace(pos, 7, "");
            pos = raw_name.find("__ptr64");
        }
        pos = raw_name.find(" ");
        while (pos != std::string::npos) {
            raw_name.replace(pos, 1, "");
            pos = raw_name.find(" ");
        }
        return raw_name;
#elif defined(MGE_COMPILER_GNUC)
        __SIZE_TYPE__ sz = 511;
        int status;
        char *buffer;
        buffer = (char *)alloca(sz + 1);
        __cxa_demangle(raw_name.c_str(), buffer, &sz, &status);
        if (status == 0) {
            return std::string(buffer);
        } else {
            switch (status) {
            case -1:
                MGE_THROW(mge::bad_alloc(),
                          "Memory allocation failed");
            case -2:
                MGE_THROW(mge::illegal_argument(),
                          "'",
                          raw_name,
                          "' is not a valid mangled type name");
            case -3:
                MGE_THROW(mge::illegal_state(),
                          Invalid arguments of demangling operation");
            default:
                MGE_THROW(mge::illegal_state(),
                          "Unknown error ",
                          status,
                          "while determining type name");
             }
       }
#else
#  error "Unknown compiler"
#endif
    }

    MGE_CORE_EXPORT std::string base_type_name(const std::type_info& ti)
    {
        std::string full_type_name = type_name(ti);
        auto namestart = full_type_name.begin();
        auto it = namestart;
        while(it != full_type_name.end()) {
            if(*it == ':') {
                ++it;
                if(it == full_type_name.end() || *it != ':') {
                    MGE_THROW(mge::illegal_state)
                              << "Bad type name: " << full_type_name;
                }
                ++it;
                namestart = it;
            } else if(!isalnum(*it) && *it!='_') {
                return std::string(namestart, full_type_name.end());
            } else {
                ++it;
            }
        }
        return std::string(namestart, full_type_name.end());
    }

    MGE_CORE_EXPORT std::string namespace_name(const std::type_info& ti)
    {
        std::string full_type_name = type_name(ti);
        auto namestart = full_type_name.begin();
        auto it = namestart;
        while(it != full_type_name.end()) {
            if(*it == ':') {
                ++it;
                if(it == full_type_name.end() || *it != ':') {
                    MGE_THROW(mge::illegal_state) << "Bad type name: " <<  full_type_name;
                }
                ++it;
                namestart = it;
            } else if(!isalnum(*it) && *it!='_') {
                if(namestart == full_type_name.begin()) {
                    return std::string();
                } else {
                    return std::string(full_type_name.begin(), namestart - 2);
                }
            } else {
                ++it;
            }
        }

        if(namestart == full_type_name.begin()) {
            return std::string();
        } else {
            return std::string(full_type_name.begin(), namestart - 2);
        }

    }

}
