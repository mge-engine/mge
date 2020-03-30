// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/types.hpp"

#include <string>

namespace mge {

    /**
     * Compute a boolean from a string.
     * @param str string to check
     * @return @c true if the string is matching case-insensitive
     * "true", "yes", "on" or if the string is a non-zero number
     */
    inline bool bool_value(const char *str) noexcept
    {
        if (str == nullptr) {
            return false;
        }

        if (*str == 't' || *str == 'T') {
            ++str;
            if (*str != 'r' && *str != 'R') {
                return false;
            }
            ++str;
            if (*str != 'u' && *str != 'U') {
                return false;
            }
            ++str;
            if (*str != 'e' && *str != 'E') {
                return false;
            }
            ++str;
            return *str == 0;
        } else if (*str == 'y' || *str == 'Y') {
            ++str;
            if (*str != 'e' && *str != 'E') {
                return false;
            }
            ++str;
            if (*str != 's' && *str != 'S') {
                return false;
            }
            ++str;
            return *str == 0;
        } else if (*str == 'o' || *str == 'O') {
            ++str;
            if (*str != 'n' && *str != 'N') {
                return false;
            }
            ++str;
            return *str == 0;
        } else if (*str >= '0' && *str <= '9') {
            int i = atoi(str);
            return i != 0;
        }
        return false;
    }

    /**
     * Compute a boolean from a string.
     * @param s string to check
     * @return @c true if the string is matching case-insensitive
     * "true", "yes", "on" or if the string is a non-zero number
     */
    inline bool bool_value(const std::string &s) noexcept
    {
        return bool_value(s.c_str());
    }

    inline bool bool_value(std::string_view s) noexcept
    {
        if (s.empty()) {
            return false;
        }

        auto str  = s.cbegin();
        auto send = s.cend();

        if (*str == 't' || *str == 'T') {
            ++str;
            if (str == send) {
                return false;
            }
            if (*str != 'r' && *str != 'R') {
                return false;
            }
            ++str;
            if (str == send) {
                return false;
            }
            if (*str != 'u' && *str != 'U') {
                return false;
            }
            ++str;
            if (str == send) {
                return false;
            }
            if (*str != 'e' && *str != 'E') {
                return false;
            }
            ++str;
            return str == send;
        } else if (*str == 'y' || *str == 'Y') {
            ++str;
            if (str == send) {
                return false;
            }
            if (*str != 'e' && *str != 'E') {
                return false;
            }
            ++str;
            if (str == send) {
                return false;
            }
            if (*str != 's' && *str != 'S') {
                return false;
            }
            ++str;
            return str == send;
        } else if (*str == 'o' || *str == 'O') {
            ++str;
            if (str == send) {
                return false;
            }
            if (*str != 'n' && *str != 'N') {
                return false;
            }
            ++str;
            return str == send;
        } else if (*str >= '0' && *str <= '9') {
            int i = std::stoi(std::string(s));
            if (i) {
                return true;
            }
        }
        return false;
    }

} // namespace mge
