// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"

#include "mge/graphics/context_object.hpp"

namespace mge {

    /**
     * @brief A sequence of state changes.
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
         * @brief Returns whether this command list is native or in-memory.
         *
         * @return @c true if native
         */
        bool native() const;

    private:
        bool m_native;
    };

} // namespace mge