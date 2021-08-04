// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include <memory>

namespace mge {

    /**
     * A render context is a context in which objects that are elements
     * of the render pipeline are created - such as shader programs,
     * textures and buffers.
     *
     * A render context has a swap chain to facilitate presentation
     * of the rendered frames.
     */
    class MGEGRAPHICS_EXPORT render_context
        : public std::enable_shared_from_this<render_context>
    {
    public:
        virtual ~render_context() = default;

        /**
         * @brief Get swap chain of context.
         *
         * @return swap chain
         */
        const swap_chain_ref &swap_chain() const;

    protected:
        render_context();

        swap_chain_ref m_swap_chain; //!< swap chain of this context
    };
} // namespace mge