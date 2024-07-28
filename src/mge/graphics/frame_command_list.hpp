#pragma once
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {
    /**
     * A command list for a frame.
     */
    class MGEGRAPHICS_EXPORT frame_command_list : public command_list
    {
    protected:
        frame_command_list(render_context& context,
                           uint32_t        backbuffer_index,
                           bool            native);

    public:
        /**
         * Indicator for not using any backbuffer.
         */
        static constexpr uint32_t NO_BACKBUFFER = 0xffffffff;

        ~frame_command_list();

        /**
         * @brief Returns the backbuffer index.
         *
         * @return backbuffer index
         */
        uint32_t backbuffer_index() const noexcept;

    private:
        uint32_t m_backbuffer_index;
    };
} // namespace mge