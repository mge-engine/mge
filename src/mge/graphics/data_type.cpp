// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include <iostream>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const data_type& t)
    {
        switch (t) {
        case data_type::UNKNOWN:
            return os << "UNKNOWN";
        case data_type::INT8:
            return os << "INT8";
        case data_type::UINT8:
            return os << "UINT8";
        case data_type::INT16:
            return os << "INT16";
        case data_type::UINT16:
            return os << "UINT16";
        case data_type::INT32:
            return os << "INT32";
        case data_type::UINT32:
            return os << "UINT32";
        case data_type::INT64:
            return os << "INT64";
        case data_type::UINT64:
            return os << "UINT64";
        case data_type::INT128:
            return os << "INT128";
        case data_type::UINT128:
            return os << "UINT128";
        case data_type::HALF:
            return os << "HALF";
        case data_type::FLOAT:
            return os << "FLOAT";
        case data_type::DOUBLE:
            return os << "DOUBLE";
        case data_type::LONG_DOUBLE:
            return os << "LONG_DOUBLE";
        case data_type::FLOAT_VEC2:
            return os << "FLOAT_VEC2";
        case data_type::FLOAT_VEC3:
            return os << "FLOAT_VEC3";
        case data_type::FLOAT_VEC4:
            return os << "FLOAT_VEC4";
        default:
            return os << "INVALID(" << static_cast<uint32_t>(t) << ")";
        }
    }
} // namespace mge