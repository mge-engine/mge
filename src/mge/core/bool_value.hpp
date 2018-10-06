// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/types.hpp"

namespace mge {
    /**
     * Compute a boolean from a string.
     * @param str string to check
     * @return @c true if the string is matching case-insensitive
     * "true", "yes", "on" or if the string is a non-zero number
     */
    inline bool bool_value(const char *str)
    {
        if(str == nullptr) {
            return false;
        }

        if(*str=='t' || *str == 'T') {
            ++str;
            if(*str != 'r' && *str != 'R') {
                return false;
            }
            ++str;
            if(*str != 'u' && *str != 'U') {
                return false;
            }
            ++str;
            if(*str != 'e' && *str != 'E') {
                return false;
            }
            ++str;
            return *str == 0;
        } else if(*str == 'y' || *str=='Y') {
            ++str;
            if(*str != 'e' && *str != 'E') {
                return false;
            }
            ++str;
            if(*str != 's' && *str != 'S') {
                return false;
            }
            ++str;
            return *str == 0;
        } else if(*str == 'o' || *str=='O') {
            if(*str != 'n' && *str != 'N') {
                return false;
            }
            ++str;
            return *str == 0;
        } else if(*str >= '0' && *str <= '9') {
            int i = atoi(str);
            return i!=0;
        }
        return false;
    }

    /**
     * Compute a boolean from a string.
     * @param str string to check
     * @return @c true if the string is matching case-insensitive
     * "true", "yes", "on" or if the string is a non-zero number
     */
    inline bool bool_value(const std::string& s)
    {
        return bool_value(s.c_str());
    }

}
