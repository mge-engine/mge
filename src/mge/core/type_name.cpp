// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/type_name.hpp"
#include "mge/config.hpp"
#include <cstring>

namespace mge {

    static inline void remove_string(std::string &str, const char *to_replace)
    {
        auto l   = strlen(to_replace);
        auto pos = str.find(to_replace);
        while (pos != std::string::npos) {
            str.replace(pos, l, "");
            pos = str.find(to_replace);
        }
    }

    MGECORE_EXPORT std::string type_name(const std::type_info &ti)
    {
        std::string raw_name(ti.name());
#ifdef MGE_COMPILER_MSVC
        remove_string(raw_name, "class ");
        remove_string(raw_name, "struct ");
        remove_string(raw_name, "enum ");
        remove_string(raw_name, "__ptr64");
        remove_string(raw_name, " ");
        return raw_name;
#else
#    error Missing port
#endif
    }

    MGECORE_EXPORT std::string base_type_name(const std::type_info &ti)
    {
        std::string full_type_name = type_name(ti);
        auto        namestart      = full_type_name.begin();
        auto        it             = namestart;
        while (it != full_type_name.end()) {
            if (*it == ':') {
                ++it;
                if (it == full_type_name.end() || *it != ':') {
                    return "";
                    // MGE_THROW(mge::illegal_state)
                    //    << "Bad type name: " << full_type_name;
                }
                ++it;
                namestart = it;
            } else if (!isalnum(*it) && *it != '_') {
                return std::string(namestart, full_type_name.end());
            } else {
                ++it;
            }
        }
        return std::string(namestart, full_type_name.end());
    }

    MGECORE_EXPORT std::string namespace_name(const std::type_info &ti)
    {
        std::string full_type_name = type_name(ti);
        auto        namestart      = full_type_name.begin();
        auto        it             = namestart;
        while (it != full_type_name.end()) {
            if (*it == ':') {
                ++it;
                if (it == full_type_name.end() || *it != ':') {
                    return "";
                    //    MGE_THROW(mge::illegal_state)
                    //    << "Bad type name: " << full_type_name;
                }
                ++it;
                namestart = it;
            } else if (!isalnum(*it) && *it != '_') {
                if (namestart == full_type_name.begin()) {
                    return std::string();
                } else {
                    return std::string(full_type_name.begin(), namestart - 2);
                }
            } else {
                ++it;
            }
        }

        if (namestart == full_type_name.begin()) {
            return std::string();
        } else {
            return std::string(full_type_name.begin(), namestart - 2);
        }
    }

} // namespace mge
