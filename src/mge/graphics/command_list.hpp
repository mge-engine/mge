// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"

#include "mge/graphics/context_object.hpp"
#include "mge/graphics/rgba_color.hpp"

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

    private:
        bool m_native;
    };

} // namespace mge