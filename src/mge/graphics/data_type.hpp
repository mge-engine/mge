#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/math/vec2.hpp"
#include "mge/math/vec3.hpp"
#include "mge/math/vec4.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {

    /**
     * A data type, e.g. to express the type of data stored in a buffer.
     */
    enum class MGE_GRAPHICS_EXPORT data_type : uint8_t
    {
        UNKNOWN = 0,    //!< Unknown or not specified
        UINT8,          //!< Unsigned 8-bit integer
        INT8,           //!< Signed 8-bit integer
        UINT16,         //!< Unsigned 16-bit integer
        INT16,          //!< Signed 16-bit integer
        HALF,           //!< Half-precision floating point
        UINT32,         //!< Unsigned 32-bit integer
        INT32,          //!< Signed 32-bit integer
        FLOAT,          //!< Single precision floating point
        UINT64,         //!< Unsigned 64-bit integer
        INT64,          //!< Signed 64-bit integer
        DOUBLE,         //!< Double precision floating point
        LONG_DOUBLE,    //!< Extended precision floating point
        UINT128,        //!< Unsigned 128-bit integer
        INT128,         //!< Signed 128-bit integer
        FLOAT_VEC2,     //!< 2-component vector of single precision floating point values
        FLOAT_VEC3,     //!< 3-component vector of single precision floating point values
        FLOAT_VEC4,     //!< 4-component vector of single precision floating point values
        LAST = FLOAT_VEC4   //!< Sentinel type
    };

    template <typename T>
    inline data_type data_type_of_type() noexcept
    {
        return mge::data_type::UNKNOWN;
    }

#define MAP_TYPE(c_type, data_type_value)           \
    template <>                                     \
    inline data_type data_type_of_type<c_type>()    \
    {                                               \
        return data_type_value;                     \
    }

    MAP_TYPE(std::uint8_t, data_type::UINT8)
    MAP_TYPE(std::int8_t, data_type::INT8)
    MAP_TYPE(std::uint16_t, data_type::UINT16)
    MAP_TYPE(std::int16_t, data_type::INT16)
    MAP_TYPE(std::uint32_t, data_type::UINT32)
    MAP_TYPE(std::int32_t, data_type::INT32)
    MAP_TYPE(float, data_type::FLOAT)
    MAP_TYPE(std::uint64_t, data_type::UINT64)
    MAP_TYPE(std::int64_t, data_type::INT64)
    MAP_TYPE(double, data_type::DOUBLE)
    MAP_TYPE(long double, data_type::LONG_DOUBLE)
    MAP_TYPE(mge::fvec2, data_type::FLOAT_VEC2)
    MAP_TYPE(mge::fvec3, data_type::FLOAT_VEC3)
    MAP_TYPE(mge::fvec4, data_type::FLOAT_VEC4)

#undef MAP_TYPE

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, const data_type t);
}
