// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/buffer.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/image.hpp"
#include "mge/graphics/image_format.hpp"

namespace mge {

    /**
     * @brief Image stored in memory.
     *
     */
    class MGEGRAPHICS_EXPORT memory_image : public image
    {
    public:
        memory_image(image_format       format_,
                     const mge::extent& extent_,
                     size_t             size = 0);

        memory_image(image_format       format_,
                     const mge::extent& extent_,
                     void*              data,
                     bool               copy_data = false,
                     size_t             size = 0);

        virtual ~memory_image();

        /** @copydoc image::data() */
        void* data() const override;

        /** @copydoc image::data_span() */
        std::span<uint8_t> data_span() const override;

        /** @copydoc image::binary_size() */
        size_t binary_size() const override;

    private:
        void*  m_data;
        size_t m_data_size;
    };
} // namespace mge