#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
namespace mge {


    /**
     * A window. A window interfaces with the operating
     * system to display information presented on its
     * device context, and to receive input when it has the
     * focus.
     *
     * A window may be _windowed_ (have a title and handles to
     * minimize/maximize/close etc.) or may be a full screen
     * window that has no borders at all.
     */
    class MGE_GRAPHICS_EXPORT window
            : public std::enable_shared_from_this<window>
    {
    protected:
        window();
    public:
        ~window();
    };

}
