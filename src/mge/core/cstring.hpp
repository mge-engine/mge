#pragma once
#include <cstring>

namespace mge {

    struct cstring_less
    {
        bool operator()(const char* lhs, const char* rhs) const
        {
            return strcmp(lhs, rhs) < 0;
        }
    };

} // namespace mge