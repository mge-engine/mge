// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/rgba_color.hpp"

namespace mge {

    /**
     * @brief A pass is a bucket for draw, resource and compute
     * commands.
     *
     */
    class MGEGRAPHICS_EXPORT pass
    {
    public:
        enum class draw_mode
        {
            DEFAULT,
            SEQUENTIAL,
            DEPTH_ASCENDING,
            DEPTH_DESCENDING
        };

        pass(const pass&) = delete;
        pass(pass&&) noexcept = default;
        pass& operator=(const pass&) = delete;
        pass& operator=(pass&&) = default;

        /**
         * @brief Default destructor.
         */
        ~pass();

        void set_rect(const mge::rectangle& r);

        void set_scissor(const mge::rectangle& r);

        void clear_color(const rgba_color& color);
        void clear_depth(float depth);
        void clear_stencil(int32_t stencil);

        void set_draw_mode(mge::pass::draw_mode mode);
        // void set_frame_buffer(mge::frame_buffer* fb);
        void reset();

        void touch();

        bool active() const noexcept
        {
            return m_active;
        }

    private:
        friend class render_context;
        pass(mge::render_context* context, uint32_t index) noexcept;

        mge::render_context* m_context{nullptr};
        uint32_t             m_index{0};
        bool                 m_active{false};
    };

} // namespace mge