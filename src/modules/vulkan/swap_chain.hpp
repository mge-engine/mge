#pragma once
#include "mge/graphics/swap_chain.hpp"
#include "render_context.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {

    class swap_chain : public mge::swap_chain
    {
    public:
        swap_chain(render_context& context);
        virtual ~swap_chain();
        void present() override;

    private:
        render_context& m_render_context;
    };

} // namespace mge::vulkan