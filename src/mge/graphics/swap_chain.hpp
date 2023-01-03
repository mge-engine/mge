// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/context_object.hpp"
namespace mge {

    /**
     * @brief A swap chain is a series of virtual frame buffers.
     *
     * Commonly, a swap chain is used for frame rate stabilization
     * by using double, or even triple buffering.
     */
    class MGEGRAPHICS_EXPORT swap_chain : public context_object
    {
    protected:
        explicit swap_chain(render_context& context);

    public:
        virtual ~swap_chain();

        /**
         * @brief Presents the next frame buffer.
         */
        virtual void present() = 0;
    };
} // namespace mge