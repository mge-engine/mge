#include "swap_chain.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    swap_chain::swap_chain(render_context& context)
        : mge::swap_chain(context)
    {}

    swap_chain::~swap_chain() {}

    void swap_chain::present() {}
} // namespace mge::vulkan