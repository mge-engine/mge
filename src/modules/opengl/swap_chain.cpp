// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/memory_image.hpp"
#include "render_context.hpp"

namespace mge::opengl {
    swap_chain::swap_chain(render_context& context)
        : mge::swap_chain(context)
        , m_hdc(context.dc())
    {}

    void swap_chain::present()
    {
#ifdef MGE_OS_WINDOWS
        SwapBuffers(m_hdc);
#else
#    error Missing port
#endif
    }

    image_ref swap_chain::screenshot()
    {
        auto& rc = static_cast<render_context&>(context());
        auto  extent = rc.extent();

        // OpenGL uses RGBA format with 8 bits per channel
        image_format format(image_format::data_format::RGBA, data_type::UINT8);
        auto         img =
            std::make_shared<memory_image>(format,
                                           extent,
                                           extent.width * extent.height * 4);

        // Read pixels from the front buffer
        glReadBuffer(GL_FRONT);
        glReadPixels(0,
                     0,
                     static_cast<GLsizei>(extent.width),
                     static_cast<GLsizei>(extent.height),
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     img->data());

        // Flip the image vertically (OpenGL origin is bottom-left, images
        // expect top-left)
        auto*  pixels = static_cast<uint8_t*>(img->data());
        size_t row_size = extent.width * 4;
        auto   temp_row = std::make_unique<uint8_t[]>(row_size);

        for (uint32_t y = 0; y < extent.height / 2; ++y) {
            uint8_t* top_row = pixels + y * row_size;
            uint8_t* bottom_row = pixels + (extent.height - 1 - y) * row_size;

            std::memcpy(temp_row.get(), top_row, row_size);
            std::memcpy(top_row, bottom_row, row_size);
            std::memcpy(bottom_row, temp_row.get(), row_size);
        }

        return img;
    }
} // namespace mge::opengl