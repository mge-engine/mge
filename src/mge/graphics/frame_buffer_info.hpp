// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/extent.hpp"
#include "mge/graphics/image_format.hpp"

#include <optional>
#include <vector>

namespace mge {

    /**
     * @brief Descriptor for creating a frame buffer with pre-specified
     * attachments.
     *
     * Pass a populated instance to
     * @c render_context::create_frame_buffer(const frame_buffer_info&) to
     * have the backend allocate all attachment resources on creation.
     * Each color attachment carries its own format and extent so that
     * multiple render targets with different sizes are supported.
     * The optional depth/stencil attachment shares its extent with
     * @c depth_stencil_extent; the concrete format is chosen by the backend
     * (typically D24S8 or D32F_S8).
     */
    struct frame_buffer_info
    {
        /**
         * @brief Specification for one color attachment slot.
         */
        struct color_attachment
        {
            image_format format; //!< pixel format of the attachment
            mge::extent  extent; //!< size in pixels
        };

        std::vector<color_attachment> color_attachments;
        std::optional<mge::extent>    depth_stencil_extent;
    };

} // namespace mge
