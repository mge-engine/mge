// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/image.hpp"
#include "mge/core/buffer.hpp"

namespace mge {
    class MGE_GRAPHICS_EXPORT memory_image : public image
    {
    public:
        memory_image(const mge::extent& e,
                     const image_format& f,
                     bool mapped = true);
        memory_image(const mge::extent& e,
                     const image_format& f,
                     void *data,
                     bool take_ownership = false);
        virtual ~memory_image();

        /**
         * Get a scan line.
         * @param line line requested, in range 0 .. height
         * @return pointer to scan line
         */
        void *scanline(uint32_t line);
    protected:
        void *on_map() override;
        void on_unmap() override;
    private:

        size_t buffer_size() const;
        uint8_t *m_data;
    };

}
