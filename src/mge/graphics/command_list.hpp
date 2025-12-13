// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"

#include "mge/graphics/context_object.hpp"
#include "mge/graphics/draw_command.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/viewport.hpp"

namespace mge {

    /**
     * @brief A sequence of state changes.
     *
     * A list of state-changing commands which can be applied to
     * a render context.
     */
    class MGEGRAPHICS_EXPORT command_list : public context_object
    {
    protected:
        /**
         * @brief Construct a new command list object
         *
         * @param context command list render context
         * @param native whether this command list is native oder
         *   in-memory
         */
        command_list(render_context& context, bool native);

    public:
        /**
         * @brief Destructor.
         */
        virtual ~command_list();

        /**
         * @brief Set viewport.
         * @param vp viewport
         */
        virtual void viewport(const mge::viewport& vp) = 0;

        /**
         * @brief Set scissor rectangle.
         * @param rect scissor rectangle
         */
        virtual void scissor(const rectangle& rect) = 0;

        /**
         * Set default scissor for the render context.
         */
        virtual void default_scissor() = 0;

        /**
         * @brief Returns whether this command list is native.
         *
         * Some render APIs support command list objects on their API level.
         * If the command list is mapped to such an object, a command list
         * is native.
         *
         * @return @c true if native
         */
        bool native() const;

        /**
         * @brief Clears the color buffer with the requested color.
         *
         * @param color clear color
         */
        virtual void clear(const rgba_color& color) = 0;

        /**
         * @brief Clears the depth buffer with the requested value.
         *
         * @param depth clear depth value
         */
        virtual void clear_depth(float depth) = 0;

        /**
         * @brief Clears the stencil buffer with the requested value.
         *
         * @param stencil clear stencil value
         */
        virtual void clear_stencil(int32_t stencil) = 0;

        /**
         * @brief Draws a draw command.
         *
         * @param command draw command
         */
        virtual void draw(const draw_command& command) = 0;

        /**
         * @brief Finishes filling the command list.
         */
        virtual void finish();

        /**
         * @brief Executes this command list.
         */
        virtual void execute() = 0;

        /**
         * @brief Destroy the command list.
         *
         * The command list is removed from the context and
         * its resources are freed. Afterwards, the pointer to the
         * command list is invalid.
         */
        void destroy();

    private:
        bool m_native;
    };

} // namespace mge