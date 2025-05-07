// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/core/format.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"

#include <cstdint>
#include <iosfwd>

namespace mge {

    /**
     * @brief Image format.
     * An image format consists of a data format and a data type.
     * The data format describes the layout of the data, e.g. RGB or RGBA.
     * The data type describes the type of the data, e.g. @c float.
     */
    class MGEGRAPHICS_EXPORT image_format
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

        data_format format() const
        {
            return m_format;
        }
        data_type type() const
        {
            return m_type;
        }

        inline constexpr size_t components() const noexcept
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

        inline size_t binary_size() const noexcept
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

} // namespace mge

template <>
struct fmt::formatter<mge::image_format>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::image_format& f, FormatContext& ctx) const
    {
        fmt::format_to(ctx.out(), "{}_{}", f.format(), f.type());
        return ctx.out();
    }
};

namespace mge {
    inline std::ostream& operator<<(std::ostream& os, const image_format& f)
    {
        os << fmt::format("{}", f);
        return os;
    }
} // namespace mge