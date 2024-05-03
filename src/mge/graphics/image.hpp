// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/image_format.hpp"

#include <span>

namespace mge {

    /**
     * An image. Base class for image types.
     *
     * An image has an extent (size) and a format.
     * Images are immutable, i.e. format and size cannot change inplace
     */
    class image
    {
    protected:
        /**
         * @brief Construct a new image object
         *
         * @param f image format
         * @param e image extent
         */
        image(image_format f, const mge::extent& e)
            : m_format(f)
            , m_extent(e)
        {}

    public:
        image(const image&) = delete;
        image& operator=(const image&) = delete;
        image(image&&) = delete;
        image& operator=(image&&) = delete;

        virtual ~image() = default;

        /**
         * @brief Get the format of the image.
         *
         * @return image_format
         */
        image_format format() const noexcept { return m_format; }

        /**
         * @brief Get the extent of the image.
         *
         * @return extent
         */
        const mge::extent& extent() const noexcept { return m_extent; }

        virtual void* data() const = 0;

        virtual std::span<uint8_t> data_span() const = 0;

        virtual size_t binary_size() const = 0;

        inline void format(std::format_context& ctx) const
        {
            std::format_to(ctx.out(),
                           "image{{format={}, extent={}}}",
                           m_format,
                           m_extent);
        }

    private:
        image_format m_format;
        mge::extent  m_extent;
    };

} // namespace mge