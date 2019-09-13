// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/rgba_color.hpp"

namespace mge {

    /**
     * A command list ist a list of state changing commands which
     * can be executed by a render context. A command list can be
     * either implemented natively, i.e. the graphics subsystem
     * implements command queues and command lists, or abstract.
     */
    class MGE_GRAPHICS_EXPORT command_list
            : public context_object
    {
    protected:
        /**
         * Constructor.
         * @param context   render context
         * @param native    flag whether this command list is native
         *                  or abstract
         */
        command_list(render_context& context, bool native=true);
    public:
        /**
         * Destructor.
         */
        virtual ~command_list();

        /**
         * Get whether this is a native command list.
         * @return @c true if native
         */
        bool native() const noexcept { return m_native; }

        /**
         * Schedules a clear command of the color buffer.
         * @param c color to use for clearing
         */
        virtual void clear(const rgba_color& c) = 0;
    private:
        bool m_native;
    };
}
