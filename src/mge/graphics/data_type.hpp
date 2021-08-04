// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

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

        FLOAT_VEC2, //!< 2-dimensional float vector
        FLOAT_VEC3, //!< 3 dimensional float vector
        FLOAT_VEC4  //!< 4 dimensional float vector
    };

    /**
     * @brief Output operator.
     *
     * @param os output stream
     * @param t data type printed
     * @return @c os
     */
    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream& os, const data_type& t);

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
            return 8;
        case data_type::FLOAT_VEC3:
            return 12;
        case data_type::FLOAT_VEC4:
            return 16;
        default:
            MGE_THROW(illegal_argument) << "Unexpected data type " << t;
        }
    }

} // namespace mge