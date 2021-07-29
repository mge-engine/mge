#pragma once
#include "mge/graphics/render_system.hpp"
#include "vulkan_library.hpp"

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

    private:
        void create_instance();
        void destroy_instance();
        void teardown();

        std::shared_ptr<vulkan_library> m_library;
        VkInstance                      m_instance;
    };

} // namespace mge::vulkan