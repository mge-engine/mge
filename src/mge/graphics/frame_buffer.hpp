// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

#include <vector>

namespace mge {

    /**
     * @brief An off-screen render target consisting of one or more
     * color attachments and an optional depth/stencil attachment.
     *
     * A frame buffer is created via
     * @c render_context::create_frame_buffer().  Attachments can be
     * specified at creation time through @c frame_buffer_info, or added
     * afterwards with @c attach_color() / @c attach_depth().
     */
    class MGEGRAPHICS_EXPORT frame_buffer : public mge::context_object
    {
    protected:
        explicit frame_buffer(render_context& context);

    public:
        virtual ~frame_buffer() = default;

        /**
         * @brief Attach a texture as a color render target.
         *
         * Calls @c on_attach_color() for backend GPU-side binding, then
         * stores @c tex in @c m_color_attachments.
         *
         * @param tex  texture to bind as color attachment
         * @param slot color attachment index (0-based)
         */
        void attach_color(texture_ref tex, uint32_t slot = 0);

        /**
         * @brief Attach a texture as the depth (or depth/stencil) target.
         *
         * Calls @c on_attach_depth() for backend GPU-side binding, then
         * stores @c tex in @c m_depth_attachment.
         *
         * @param tex texture to bind as depth attachment
         */
        void attach_depth(texture_ref tex);

        /**
         * @brief Retrieve a color attachment by slot.
         *
         * @param slot attachment index
         * @return texture_ref, empty if slot has not been attached
         */
        texture_ref color_attachment(uint32_t slot) const;

        /**
         * @brief Retrieve the depth attachment.
         *
         * @return texture_ref, empty if no depth attachment has been set
         */
        texture_ref depth_attachment() const;

    protected:
        /**
         * @brief Backend hook called by @c attach_color() before the base
         * class updates @c m_color_attachments.
         *
         * @param tex  texture being attached
         * @param slot color attachment index (0-based)
         */
        virtual void on_attach_color(texture_ref tex, uint32_t slot) = 0;

        /**
         * @brief Backend hook called by @c attach_depth() before the base
         * class updates @c m_depth_attachment.
         *
         * @param tex texture being attached
         */
        virtual void on_attach_depth(texture_ref tex) = 0;

        std::vector<texture_ref> m_color_attachments;
        texture_ref              m_depth_attachment;
    };

} // namespace mge