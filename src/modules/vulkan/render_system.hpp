#pragma once
#include "vulkan.hpp"
#include "mge/graphics/render_system.hpp"
namespace vulkan {

    class render_system : public mge::render_system
    {
    public:
        render_system();
        ~render_system();
    };

}