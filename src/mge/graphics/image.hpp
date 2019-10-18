// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/image_format.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/core/types.hpp"

namespace mge {

    /**
     * An image.
     */
    class MGE_GRAPHICS_EXPORT image : noncopyable
    {
    protected:
        /**
         * Constructor.
         * @param extent extent of image
         * @param format image format
         */
        image(const extent& extent,
              const image_format& format);
    public:
        /**
         * Destructor.
         */
        virtual ~image() = default;

        /**
         * Copies the image into a new one (which is
         * a memory image)
         * @return copied image
         */
        image_ref clone();

        /**
         * Maps the image data.
         * @return image data
         */
        void *map();

        /**
         * Unmaps the image data. The pointer returned by
         * a previous @c map call may become invalid.
         */
        void unmap();

        /**
         * Return image extent.
         * @return image extent
         */
        const extent& extent() const { return m_extent; }

        /**
         * Return image format.
         * @return image format
         */
        image_format format() const { return m_format; }
    protected:
        virtual void *on_map() = 0;
        virtual void on_unmap() = 0;
    private:
        mge::extent m_extent;
        image_format m_format;
    };
}
