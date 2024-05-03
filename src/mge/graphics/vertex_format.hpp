// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"

#include <string_view>

namespace mge {

    /**
     * @brief A vertex format describes the format of one vertex.
     *
     */
    class MGEGRAPHICS_EXPORT vertex_format
    {
    public:
        /**
         * @brief Default constructor.
         * Creates an unknown vertex.
         */
        vertex_format();
        /**
         * @brief Construct a new vertex format object
         *
         * @param type data type
         * @param size number of elements, default 1
         */
        vertex_format(data_type type, size_t size = 1);

        /**
         * @brief Copy constructor.
         *
         * @param f copied format
         */
        vertex_format(const vertex_format& f) = default;

        /**
         * @brief Move constructor.
         *
         * @param f moved format
         */
        vertex_format(vertex_format&& f) = default;

        /**
         * @brief Assignment.
         *
         * @param f assigned format
         * @return @c *this
         */
        vertex_format& operator=(const vertex_format& f) = default;

        /**
         * @brief Move assignment,
         *
         * @param f assigned format
         * @return @c *this
         */
        vertex_format& operator=(vertex_format&& f) = default;

        /**
         * @brief Data type of vertex.
         *
         * @return data type
         */
        data_type type() const noexcept { return m_type; }

        /**
         * @brief Number of elements in vertex.
         *
         * @return number of elements
         */
        uint8_t size() const noexcept { return m_size; }

        /**
         * @brief Binary size of vertex (number of bytes)
         *
         * @return binary size
         */
        size_t binary_size() const;

        /**
         * @brief Comparison operator.
         *
         * @param f compared format
         * @return @c true if this is less than @c f
         */
        inline bool operator<(const vertex_format& f) const noexcept
        {
            return m_type < f.m_type    ? true
                   : m_type == f.m_type ? m_size < f.m_size
                                        : false;
        }

        /**
         * @brief Comparison operator.
         *
         * @param f compared format
         * @return @c true if this is equal to @c f
         */
        inline bool operator==(const vertex_format& f) const noexcept
        {
            return m_type == f.m_type && m_size == f.m_size;
        }

        /**
         * @brief Format value for output.
         * @param ctx format context
         */
        void format(std::format_context& context) const;

    private:
        data_type m_type;
        uint8_t   m_size;
    };

    /**
     * @brief Parse a vertex format.
     *
     * @param sv source string view
     * @return parsed format
     */
    MGEGRAPHICS_EXPORT vertex_format parse_vertex_format(std::string_view sv);

} // namespace mge

template <> struct std::hash<mge::vertex_format>
{
    size_t operator()(const mge::vertex_format& fmt) const noexcept
    {
        return static_cast<size_t>(fmt.type()) ^
               static_cast<size_t>(fmt.size());
    }
};
