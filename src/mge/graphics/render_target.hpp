#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/core/async_executor.hpp"
#include <memory>
namespace mge {

    class MGE_GRAPHICS_EXPORT render_target
            : public std::enable_shared_from_this<render_target>
            , public async_executor
    {
    protected:
        render_target(async_executor *display_executor);
    public:
        virtual ~render_target() = default;
    protected:
        async_executor *m_display_executor;
    };
}
