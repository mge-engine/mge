#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/math/vec2.hpp"
#include "mge/math/vec3.hpp"
#include "mge/math/vec4.hpp"
#include <cstdint>
#include <iosfwd>
#include <bitset>
#include <initializer_list>

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

    template <data_type T>
    struct data_type_traits
    {
        typedef void value_type;
    };


#define MAP_TYPE(c_type, data_type_value)           \
    template <>                                     \
    inline constexpr data_type data_type_of_type<c_type>()    \
    {                                               \
        return data_type_value;                     \
    }                                               \
                                                    \
    template <>                                     \
    struct data_type_traits<data_type_value>        \
    {                                               \
        typedef c_type value_type;                  \
    };


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

    inline constexpr size_t data_type_size(const data_type t)
    {
        switch(t) {
#define RETURN_TYPESIZE(t) case t: return sizeof(data_type_traits<t>::value_type)
        RETURN_TYPESIZE(data_type::UINT8);
        RETURN_TYPESIZE(data_type::INT8);
        RETURN_TYPESIZE(data_type::UINT16);
        RETURN_TYPESIZE(data_type::INT16);
        RETURN_TYPESIZE(data_type::UINT32);
        RETURN_TYPESIZE(data_type::INT32);
        RETURN_TYPESIZE(data_type::UINT64);
        RETURN_TYPESIZE(data_type::INT64);
        RETURN_TYPESIZE(data_type::FLOAT);
        RETURN_TYPESIZE(data_type::DOUBLE);
        RETURN_TYPESIZE(data_type::LONG_DOUBLE);
        RETURN_TYPESIZE(data_type::FLOAT_VEC2);
        RETURN_TYPESIZE(data_type::FLOAT_VEC3);
        RETURN_TYPESIZE(data_type::FLOAT_VEC4);
#undef RETURN_TYPESIZE
        default:
            return 0;
        }
    }

    MGE_GRAPHICS_EXPORT std::ostream& operator <<(std::ostream& os, const data_type t);

    /**
     * Set of data types. Used to e.g. define supported set of types
     * for certain operations.
     */
    class MGE_GRAPHICS_EXPORT data_type_set
    {
    private:
        typedef std::bitset<static_cast<uint8_t>(data_type::LAST)> container_type;
    public:
        /**
         * Proxy type to allow reference to single type.
         */
        typedef container_type::reference reference;
        /**
         * Construct empty set.
         */
        data_type_set();

        /**
         * Copy constructor.
         * @param s copied set
         */
        data_type_set(const data_type_set& s);

        /**
         * Construct from initializer list.
         * @param types initializer values
         */
        data_type_set(const std::initializer_list<data_type>& types);

        /**
         * Destructor.
         */
        ~data_type_set();
        /**
         * Accesses specific type. Behaviour is undefined for
         * out of range or @c UNKNOWN data type.
         * @param t type to check
         * @return accessor to type value.
         */
        reference operator [](data_type t);

        /**
         * Retrieve value for type. Behaviour is undefined for
         * out of range or @c UNKNOWN data type.
         * @param t type to check
         * @return @c true if set
         */
        bool operator[](data_type t) const;

        /**
         * Retrieve value for type. Exception is throw for
         * out of range or @c UNKNOWN data type.
         * @param t type to check
         * @return @c true if set
         */
        bool test(data_type t) const;

        /**
         * Assignment.
         * @param s assigned set
         * @return @c *this
         */
        data_type_set& operator =(const data_type_set& s);


        /**
         * Comparison operator.
         * @param s other set
         * @return @c true if equal
         */
        bool operator ==(const data_type_set& s) const;

        /**
         * Comparison operator.
         * @param s other set
         * @return @c true if not equal
         */
        bool operator !=(const data_type_set& s) const;
    private:
        container_type m_data;
    };
}
