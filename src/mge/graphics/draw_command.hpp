#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
namespace mge {

    /**
     * @brief A draw command executes a pipeline with a given set of buffers.
     */
    class MGEGRAPHICS_EXPORT draw_command
    {
    public:
        /**
         * @brief Construct empty draw command.
         */
        draw_command();
        ~draw_command();

    private:
    };

} // namespace mge