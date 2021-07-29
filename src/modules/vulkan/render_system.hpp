#pragma once
#include "mge/graphics/render_system.hpp"
namespace mge::vulkan {

    class render_system : public mge::render_system
    {
    public:
        render_system();
        ~render_system() = default;

        mge::window_ref
        create_window(const mge::extent &        extent,
                      const mge::window_options &options) override;

        mge::render_system::monitor_collection monitors() override;
    };

} // namespace mge::vulkan