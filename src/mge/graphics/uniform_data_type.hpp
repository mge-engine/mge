#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/dllexport.hpp"
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

        FLOAT_VEC2, //!< float vector 2 elements
        FLOAT_VEC3, //!< float vector 3 elements
        FLOAT_VEC4, //!< float vector 4 elements

        FLOAT_MAT4, //!< float 4x4 matrix
    };

    /**
     * @brief Output operator.
     *
     * @param os output stream
     * @param t data type printed
     * @return @c os
     */
    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&            os,
                                                const uniform_data_type& t);

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
        inline uniform_data_type operator""_type(const char* s, size_t l)
        {
            return parse_uniform_data_type(std::string_view(s, s + l));
        }
    } // namespace literals

} // namespace mge