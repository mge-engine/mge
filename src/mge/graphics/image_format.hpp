// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"

#include <cstdint>
#include <format>
#include <iosfwd>

namespace mge {

    /**
     * @brief Image format.
     * An image format consists of a data format and a data type.
     * The data format describes the layout of the data, e.g. RGB or RGBA.
     * The data type describes the type of the data, e.g. @c float.
     */
    class image_format
    {
    public:
        enum class data_format : uint8_t
        {
            UNKNOWN = 0,
            RGB,
            RGBA,
            MAX_FORMAT = RGBA
        };

        image_format()
            : m_format(image_format::data_format::UNKNOWN)
            , m_type(data_type::UNKNOWN)
        {}

        image_format(data_format f, data_type t)
            : m_format(f)
            , m_type(t)
        {}

        data_format format() const { return m_format; }
        data_type   type() const { return m_type; }

        inline constexpr size_t components() noexcept
        {
            switch (m_format) {
            case data_format::RGB:
                return 3;
            case data_format::RGBA:
                return 4;
            default:
                return 0;
            }
        }

        inline size_t binary_size() noexcept
        {
            return components() * data_type_size(m_type);
        }

        inline constexpr bool operator==(const image_format& other) const
        {
            return m_format == other.m_format && m_type == other.m_type;
        }

        inline constexpr bool operator!=(const image_format& other) const
        {
            return m_format != other.m_format || m_type != other.m_type;
        }

    private:
        data_format m_format;
        data_type   m_type;
    };

    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&       os,
                                                const image_format& f);

} // namespace mge

template <typename C>
struct std::formatter<mge::image_format, C>
    : std::formatter<std::string_view, C>
{

    auto format(const mge::image_format& f, auto& ctx) const
        -> std::format_context::iterator
    {
        return std::format_to(ctx.out(), "test {}", 1);
#if 0
        switch (f.format()) {
        case mge::image_format::data_format::RGB:
            return std::format_to(ctx.out(), "RGB_{}", f.type());
        case mge::image_format::data_format::RGBA:
            return std::format_to(ctx.out(), "RGBA_{}", f.type());
        case mge::image_format::data_format::UNKNOWN:
            return std::format_to(ctx.out(), "UNKNOWN_{}", f.type());
        default:
            return std::format_to(ctx.out(),
                                  "INVALD({})_{}",
                                  static_cast<int>(f.format()),
                                  f.type());
        }
#endif
    }
};
