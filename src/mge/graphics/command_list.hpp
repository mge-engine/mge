#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/rgba_color.hpp"

namespace mge {

    class MGE_GRAPHICS_EXPORT command_list
            : public context_object
    {
    public:
        command_list(render_context& context, bool native=true);
        virtual ~command_list();

        bool native() const noexcept { return m_native; }

        virtual void clear(const rgba_color& c) = 0;
    private:
        bool m_native;
    };
}
