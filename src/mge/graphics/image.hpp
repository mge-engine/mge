// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/image_format.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/core/types.hpp"
#include "mge/graphics/rgba_color.hpp"

#include <atomic>

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
         * Get size of image data buffer, depends on size and format.
         * @return size in bytes of image
         */
        size_t buffer_size() const;

        /**
         * Return image format.
         * @return image format
         */
        image_format format() const { return m_format; }

        /**
         * Return whether image is mapped.
         * @return @c true if mapped
         */
        bool mapped() const { return m_map_counter !=  0; }

        void *data() const { return m_map_ptr; }

        /**
         * Return a scanline.
         * @param line line index
         * @return pointer to scanline
         */
        void *scanline(uint32_t line) const;

        /**
         * Get color of a pixel.
         * @param x x position
         * @param y y position
         * @return pixel color
         */
        rgba_color pixel_color(uint32_t x, uint32_t y) const;
    protected:
        virtual void *on_map() = 0;
        virtual void on_unmap() = 0;
    private:
        mge::extent           m_extent;
        image_format          m_format;
    protected:
        void                 *m_map_ptr;
        std::atomic<uint32_t> m_map_counter;
    };
}
