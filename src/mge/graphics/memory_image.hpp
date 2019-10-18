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
        memory_image(const mge::extent& e, const image_format& f);
        memory_image(const mge::extent& e, const image_format& f, void *data);
        virtual ~memory_image();
    protected:
        void *on_map() override;
        void on_unmap() override;
    private:
        size_t buffer_size() const;
        buffer m_data;
    };

}
