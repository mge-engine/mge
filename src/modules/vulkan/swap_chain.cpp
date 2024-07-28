#include "swap_chain.hpp"

namespace mge::vulkan {

    swap_chain::swap_chain(render_context& context)
        : mge::swap_chain(context)
        , m_render_context(context)
    {}

    swap_chain::~swap_chain() {}

    void swap_chain::present() {}

    uint32_t swap_chain::current_back_buffer_index() const { return 0; }

} // namespace mge::vulkan