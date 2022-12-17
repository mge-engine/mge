// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform_data_type.hpp"
#include <iostream>
#include <map>
#include <string_view>

namespace mge {
    std::ostream& operator<<(std::ostream& os, const uniform_data_type& t)
    {
        switch (t) {
        case uniform_data_type::UNKNOWN:
            return os << "unknown";
        case uniform_data_type::INT8:
            return os << "int8";
        case uniform_data_type::UINT8:
            return os << "uint8";
        case uniform_data_type::INT16:
            return os << "int16";
        case uniform_data_type::UINT16:
            return os << "uint16";
        case uniform_data_type::INT32:
            return os << "int32";
        case uniform_data_type::UINT32:
            return os << "uint32";
        case uniform_data_type::INT64:
            return os << "int64";
        case uniform_data_type::UINT64:
            return os << "uint64";
        case uniform_data_type::INT128:
            return os << "int128";
        case uniform_data_type::UINT128:
            return os << "uint128";
        case uniform_data_type::HALF:
            return os << "half";
        case uniform_data_type::FLOAT:
            return os << "float";
        case uniform_data_type::DOUBLE:
            return os << "double";
        case uniform_data_type::LONG_DOUBLE:
            return os << "long double";
        case uniform_data_type::FLOAT_VEC2:
            return os << "fvec2";
        case uniform_data_type::FLOAT_VEC3:
            return os << "fvec3";
        case uniform_data_type::FLOAT_VEC4:
            return os << "fvec4";
        case uniform_data_type::FLOAT_MAT4:
            return os << "fmat4";
        default:
            return os << "invalid(" << static_cast<uint32_t>(t) << ")";
        }
    }

    uniform_data_type parse_uniform_data_type(std::string_view sv)
    {
        using namespace std::literals;
        const std::map<std::string_view, uniform_data_type> types = {
            {"int8"sv, uniform_data_type::INT8},
            {"int16"sv, uniform_data_type::INT16},
            {"int32"sv, uniform_data_type::INT32},
            {"int64"sv, uniform_data_type::INT64},
            {"int128"sv, uniform_data_type::INT128},
            {"uint8"sv, uniform_data_type::UINT8},
            {"uint16"sv, uniform_data_type::UINT16},
            {"uint32"sv, uniform_data_type::UINT32},
            {"uint64"sv, uniform_data_type::UINT64},
            {"uint128"sv, uniform_data_type::UINT128},
            {"half"sv, uniform_data_type::HALF},
            {"float"sv, uniform_data_type::FLOAT},
            {"double"sv, uniform_data_type::DOUBLE},
            {"long double"sv, uniform_data_type::LONG_DOUBLE},
            {"fvec2"sv, uniform_data_type::FLOAT_VEC2},
            {"fvec3"sv, uniform_data_type::FLOAT_VEC3},
            {"fvec4"sv, uniform_data_type::FLOAT_VEC4},
            {"fmat4"sv, uniform_data_type::FLOAT_MAT4},
        };
        auto it = types.find(sv);
        if (it != types.end()) {
            return it->second;
        }

        MGE_THROW(illegal_argument) << "Invalid data type definition: " << sv;
    }

} // namespace mge