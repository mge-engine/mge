// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
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

        /**
         * @brief Destroy the frame command list.
         *
         * The frame command list is removed from the context and
         * its resources are freed. Afterwards, the pointer to the
         * frame command list is invalid.
         */
        void destroy();

    private:
        uint32_t m_backbuffer_index;
    };
} // namespace mge