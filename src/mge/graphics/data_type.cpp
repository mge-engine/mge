#include "mge/graphics/data_type.hpp"
#include <iostream>
namespace mge {
    std::ostream& operator <<(std::ostream& os, const data_type t)
    {
        if(t <= data_type::LAST)  {
            const char *values[] = {
                "UNKNOWN", "UINT8", "INT8", "UINT16", "INT16", "HALF",
                "UINT32", "INT32", "FLOAT", "UINT64", "INT64", "DOUBLE",
                "LONG_DOUBLE", "UINT128", "INT128", "FLOAT_VEC2",
                "FLOAT_VEC3", "FLOAT_VEC4"
            };
            return os << values[(uint8_t)t];
        } else {
            return os << "INVALID(" << (uint8_t)t << ")";
        }
    }
}
