#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/core/async_executor.hpp"
#include <memory>
namespace mge {

    /**
     * A render context is a context in which objects that are elements
     * of the render pipeline are created - such as shader programs,
     * textures and buffers.
     */
    class MGE_GRAPHICS_EXPORT render_context
            : public std::enable_shared_from_this<render_context>
            , public async_executor
    {
    protected:
        render_context(async_executor *display_executor);
    public:
        virtual ~render_context() = default;
        void await(const std::function<void()>& f) override;
    protected:
        async_executor *m_display_executor;
    };
}
