// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/math/mat.hpp"
#include "mge/math/vec.hpp"
#include <cstdint>
#include <iosfwd>
#include <string_view>

namespace mge {

    /**
     * @brief Data type.
     */
    enum class MGEGRAPHICS_EXPORT uniform_data_type : uint8_t
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
    inline size_t uniform_data_type_size(uniform_data_type t)
    {
        switch (t) {
        case uniform_data_type::UINT8:
        case uniform_data_type::INT8:
            return 1;
        case uniform_data_type::INT16:
        case uniform_data_type::UINT16:
            return 2;
        case uniform_data_type::INT32:
        case uniform_data_type::UINT32:
            return 4;
        case uniform_data_type::INT64:
        case uniform_data_type::UINT64:
            return 8;
        case uniform_data_type::INT128:
        case uniform_data_type::UINT128:
            return 16;
        case uniform_data_type::HALF:
            return 2;
        case uniform_data_type::FLOAT:
            return 4;
        case uniform_data_type::DOUBLE:
            return 8;
        case uniform_data_type::LONG_DOUBLE:
            return sizeof(long double);
        case uniform_data_type::FLOAT_VEC2:
            return 2 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_VEC3:
            return 3 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_VEC4:
            return 4 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT2:
            return 2 * 2 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT3:
            return 3 * 3 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT4:
            return 4 * 4 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT2x3:
            return 2 * 3 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT2x4:
            return 2 * 4 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT3x2:
            return 3 * 2 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT3x4:
            return 3 * 4 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT4x2:
            return 4 * 2 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::FLOAT_MAT4x3:
            return 4 * 3 * uniform_data_type_size(uniform_data_type::FLOAT);
        case uniform_data_type::INT_VEC2:
            return 2 * uniform_data_type_size(uniform_data_type::INT32);
        case uniform_data_type::INT_VEC3:
            return 3 * uniform_data_type_size(uniform_data_type::INT32);
        case uniform_data_type::INT_VEC4:
            return 4 * uniform_data_type_size(uniform_data_type::INT32);
        case uniform_data_type::UINT_VEC2:
            return 2 * uniform_data_type_size(uniform_data_type::UINT32);
        case uniform_data_type::UINT_VEC3:
            return 3 * uniform_data_type_size(uniform_data_type::UINT32);
        case uniform_data_type::UINT_VEC4:
            return 4 * uniform_data_type_size(uniform_data_type::UINT32);
        case uniform_data_type::DOUBLE_VEC2:
            return 2 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_VEC3:
            return 3 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_VEC4:
            return 4 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT2:
            return 2 * 2 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT3:
            return 3 * 3 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT4:
            return 4 * 4 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT2x3:
            return 2 * 3 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT2x4:
            return 2 * 4 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT3x2:
            return 3 * 2 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT3x4:
            return 3 * 4 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT4x2:
            return 4 * 2 * uniform_data_type_size(uniform_data_type::DOUBLE);
        case uniform_data_type::DOUBLE_MAT4x3:
            return 4 * 3 * uniform_data_type_size(uniform_data_type::DOUBLE);
        default:
            MGE_THROW(illegal_argument) << "Unexpected data type " << t;
        }
    }

    /**
     * @brief Parse data type from string view.
     *
     * @param sv string view
     * @return parsed data type
     */
    MGEGRAPHICS_EXPORT uniform_data_type
    parse_uniform_data_type(std::string_view sv);

    namespace literals {
        inline uniform_data_type operator""_uniform_type(const char* s,
                                                         size_t      l)
        {
            return parse_uniform_data_type(std::string_view(s, s + l));
        }
    } // namespace literals

    template <typename T> struct uniform_data_type_of
    {
        static constexpr uniform_data_type value = uniform_data_type::UNKNOWN;
    };

    template <> struct uniform_data_type_of<int8_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::INT8;
    };
    template <> struct uniform_data_type_of<uint8_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::UINT8;
    };
    template <> struct uniform_data_type_of<int16_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::INT16;
    };
    template <> struct uniform_data_type_of<uint16_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::UINT16;
    };
    template <> struct uniform_data_type_of<int32_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::INT32;
    };
    template <> struct uniform_data_type_of<uint32_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::UINT32;
    };
    template <> struct uniform_data_type_of<int64_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::INT64;
    };
    template <> struct uniform_data_type_of<uint64_t>
    {
        static constexpr uniform_data_type value = uniform_data_type::UINT64;
    };

    template <> struct uniform_data_type_of<float>
    {
        static constexpr uniform_data_type value = uniform_data_type::FLOAT;
    };
    template <> struct uniform_data_type_of<double>
    {
        static constexpr uniform_data_type value = uniform_data_type::DOUBLE;
    };

    template <> struct uniform_data_type_of<long double>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::LONG_DOUBLE;
    };

    template <> struct uniform_data_type_of<fvec2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_VEC2;
    };

    template <> struct uniform_data_type_of<fvec3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_VEC3;
    };

    template <> struct uniform_data_type_of<fvec4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_VEC4;
    };

    template <> struct uniform_data_type_of<dvec2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_VEC2;
    };

    template <> struct uniform_data_type_of<dvec3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_VEC3;
    };

    template <> struct uniform_data_type_of<dvec4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_VEC4;
    };

    template <> struct uniform_data_type_of<ivec2>
    {
        static constexpr uniform_data_type value = uniform_data_type::INT_VEC2;
    };

    template <> struct uniform_data_type_of<ivec3>
    {
        static constexpr uniform_data_type value = uniform_data_type::INT_VEC3;
    };

    template <> struct uniform_data_type_of<ivec4>
    {
        static constexpr uniform_data_type value = uniform_data_type::INT_VEC4;
    };

    template <> struct uniform_data_type_of<uvec2>
    {
        static constexpr uniform_data_type value = uniform_data_type::UINT_VEC2;
    };

    template <> struct uniform_data_type_of<uvec3>
    {
        static constexpr uniform_data_type value = uniform_data_type::UINT_VEC3;
    };

    template <> struct uniform_data_type_of<uvec4>
    {
        static constexpr uniform_data_type value = uniform_data_type::UINT_VEC4;
    };

    template <> struct uniform_data_type_of<fmat2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT2;
    };

    template <> struct uniform_data_type_of<fmat3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT3;
    };

    template <> struct uniform_data_type_of<fmat4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT4;
    };

    template <> struct uniform_data_type_of<fmat2x3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT2x3;
    };

    template <> struct uniform_data_type_of<fmat2x4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT2x4;
    };

    template <> struct uniform_data_type_of<fmat3x2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT3x2;
    };

    template <> struct uniform_data_type_of<fmat3x4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT3x4;
    };

    template <> struct uniform_data_type_of<fmat4x2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT4x2;
    };

    template <> struct uniform_data_type_of<fmat4x3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::FLOAT_MAT4x3;
    };

    template <> struct uniform_data_type_of<dmat2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT2;
    };

    template <> struct uniform_data_type_of<dmat3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT3;
    };

    template <> struct uniform_data_type_of<dmat4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT4;
    };

    template <> struct uniform_data_type_of<dmat2x3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT2x3;
    };

    template <> struct uniform_data_type_of<dmat2x4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT2x4;
    };

    template <> struct uniform_data_type_of<dmat3x2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT3x2;
    };

    template <> struct uniform_data_type_of<dmat3x4>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT3x4;
    };

    template <> struct uniform_data_type_of<dmat4x2>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT4x2;
    };

    template <> struct uniform_data_type_of<dmat4x3>
    {
        static constexpr uniform_data_type value =
            uniform_data_type::DOUBLE_MAT4x3;
    };

} // namespace mge