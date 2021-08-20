#include "dx12.hpp"
#include "mge/graphics/render_system.hpp"

namespace mge::dx12 {
    class render_system : public mge::render_system
    {
    public:
        render_system();
        virtual ~render_system() = default;

        monitor_collection monitors();
        mge::window_ref    create_window(const mge::extent&         extent,
                                         const mge::window_options& options);
    };
} // namespace mge::dx12