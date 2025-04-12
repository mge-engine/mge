// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <string_view>

namespace mge {

    /**
     * @brief Data type.
     */
    enum class MGEGRAPHICS_EXPORT data_type : uint8_t
    {
        UNKNOWN = 0, //!< unknown type
        INT8,        //!< int8_t
        UINT8,       //!< uint8_t
        INT16,       //!< int16_t
        UINT16,      //!< uint16_t
        INT32,       //!< int32_t
        UINT32,      //!< uint32_t
        INT64,       //!< int64_t
        UINT64,      //!< uint64_t
        INT128,      //!< int128_t
        UINT128,     //!< int128_t

        HALF,        //!< half precision float
        FLOAT,       //!< float
        DOUBLE,      //!< double
        LONG_DOUBLE, //!< long double

        FLOAT_VEC2, //!< 2D vector of float
        FLOAT_VEC3, //!< 3D vector of float
        FLOAT_VEC4, //!< 4D vector of float

        FLOAT_MAT2,   //!< 2x2 matrix of float
        FLOAT_MAT3,   //!< 3x3 matrix of float
        FLOAT_MAT4,   //!< 4x4 matrix of float
        FLOAT_MAT2x3, //!< 2x3 matrix of float
        FLOAT_MAT2x4, //!< 2x4 matrix of float
        FLOAT_MAT3x2, //!< 3x2 matrix of float
        FLOAT_MAT3x4, //!< 3x4 matrix of float
        FLOAT_MAT4x2, //!< 4x2 matrix of float
        FLOAT_MAT4x3, //!< 4x3 matrix of float

        INT_VEC2, //!< 2D vector of int
        INT_VEC3, //!< 3D vector of int
        INT_VEC4, //!< 4D vector of int

        UINT_VEC2, //!< 2D vector of uint
        UINT_VEC3, //!< 3D vector of uint
        UINT_VEC4, //!< 4D vector of uint

        DOUBLE_VEC2, //!< 2D vector of double
        DOUBLE_VEC3, //!< 3D vector of double
        DOUBLE_VEC4, //!< 4D vector of double

        DOUBLE_MAT2, //!< 2x2 matrix of double
        DOUBLE_MAT3, //!< 3x3 matrix of double
        DOUBLE_MAT4, //!< 4x4 matrix of double

        DOUBLE_MAT2x3, //!< 2x3 matrix of double
        DOUBLE_MAT2x4, //!< 2x4 matrix of double
        DOUBLE_MAT3x2, //!< 3x2 matrix of double
        DOUBLE_MAT3x4, //!< 3x4 matrix of double
        DOUBLE_MAT4x2, //!< 4x2 matrix of double
        DOUBLE_MAT4x3, //!< 4x3 matrix of double
    };

    /**
     * @brief Get the size of one data
     *
     * @param t
     * @return size_t
     */
    inline size_t data_type_size(data_type t)
    {
        switch (t) {
        case data_type::UINT8:
        case data_type::INT8:
            return 1;
        case data_type::INT16:
        case data_type::UINT16:
            return 2;
        case data_type::INT32:
        case data_type::UINT32:
            return 4;
        case data_type::INT64:
        case data_type::UINT64:
            return 8;
        case data_type::INT128:
        case data_type::UINT128:
            return 16;
        case data_type::HALF:
            return 2;
        case data_type::FLOAT:
            return 4;
        case data_type::DOUBLE:
            return 8;
        case data_type::LONG_DOUBLE:
            return sizeof(long double);
        case data_type::FLOAT_VEC2:
            return 2 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_VEC3:
            return 3 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_VEC4:
            return 4 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT2:
            return 2 * 2 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT3:
            return 3 * 3 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT4:
            return 4 * 4 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT2x3:
            return 2 * 3 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT2x4:
            return 2 * 4 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT3x2:
            return 3 * 2 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT3x4:
            return 3 * 4 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT4x2:
            return 4 * 2 * data_type_size(data_type::FLOAT);
        case data_type::FLOAT_MAT4x3:
            return 4 * 3 * data_type_size(data_type::FLOAT);
        case data_type::INT_VEC2:
            return 2 * data_type_size(data_type::INT32);
        case data_type::INT_VEC3:
            return 3 * data_type_size(data_type::INT32);
        case data_type::INT_VEC4:
            return 4 * data_type_size(data_type::INT32);
        case data_type::UINT_VEC2:
            return 2 * data_type_size(data_type::UINT32);
        case data_type::UINT_VEC3:
            return 3 * data_type_size(data_type::UINT32);
        case data_type::UINT_VEC4:
            return 4 * data_type_size(data_type::UINT32);
        case data_type::DOUBLE_VEC2:
            return 2 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_VEC3:
            return 3 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_VEC4:
            return 4 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT2:
            return 2 * 2 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT3:
            return 3 * 3 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT4:
            return 4 * 4 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT2x3:
            return 2 * 3 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT2x4:
            return 2 * 4 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT3x2:
            return 3 * 2 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT3x4:
            return 3 * 4 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT4x2:
            return 4 * 2 * data_type_size(data_type::DOUBLE);
        case data_type::DOUBLE_MAT4x3:
            return 4 * 3 * data_type_size(data_type::DOUBLE);
        default:
            MGE_THROW(illegal_argument) << "Unexpected data type " << t;
        }
    }

    namespace literals {
        inline data_type operator""_type(const char* s, size_t l)
        {
            auto sv = std::string_view(s, s + l);
            auto v = mge::enum_cast<data_type>(sv);
            if (v.has_value()) {
                return v.value();
            }
            MGE_THROW(illegal_argument)
                << "Invalid data type definition: " << sv;
        }
    } // namespace literals

    template <typename T> struct data_type_of
    {
        static constexpr data_type value = data_type::UNKNOWN;
    };
    template <> struct data_type_of<int8_t>
    {
        static constexpr data_type value = data_type::INT8;
    };
    template <> struct data_type_of<uint8_t>
    {
        static constexpr data_type value = data_type::UINT8;
    };
    template <> struct data_type_of<int16_t>
    {
        static constexpr data_type value = data_type::INT16;
    };
    template <> struct data_type_of<uint16_t>
    {
        static constexpr data_type value = data_type::UINT16;
    };
    template <> struct data_type_of<int32_t>
    {
        static constexpr data_type value = data_type::INT32;
    };
    template <> struct data_type_of<uint32_t>
    {
        static constexpr data_type value = data_type::UINT32;
    };
    template <> struct data_type_of<int64_t>
    {
        static constexpr data_type value = data_type::INT64;
    };
    template <> struct data_type_of<uint64_t>
    {
        static constexpr data_type value = data_type::UINT64;
    };
#if 0    
    template <> struct data_type_of<int128_t>
    {
        static constexpr data_type value = data_type::INT128;
    };
    template <> struct data_type_of<uint128_t>
    {
        static constexpr data_type value = data_type::UINT128;
    };
#endif
    template <> struct data_type_of<float>
    {
        static constexpr data_type value = data_type::FLOAT;
    };
    template <> struct data_type_of<double>
    {
        static constexpr data_type value = data_type::DOUBLE;
    };
    template <> struct data_type_of<long double>
    {
        static constexpr data_type value = data_type::LONG_DOUBLE;
    };

} // namespace mge