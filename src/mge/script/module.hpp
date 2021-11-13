// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/sfinae.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type.hpp"
#include <map>
#include <string>
#include <string_view>

namespace mge::script {

    /**
     * @brief A module of reflected types.
     *
     * A module is a name space or grouping of types, functions and other
     * modules.
     */
    class MGESCRIPT_EXPORT module
    {
    public:
        /**
         * Root module.
         */
        module();
        /**
         * Create module by (possibly scoped) identifier.
         *
         * @param identifier module identifier, in format
         * <tt>module::submodule::...</tt>
         */
        module(const std::string_view identifier);

        /**
         * Create module with existing details.
         *
         * @param details module details
         */
        module(const module_details_ref& details);

        /**
         * @brief Copy constructor.
         */
        module(const module&) = default;

        /**
         * Move constructor.
         */
        module(module&&) = default;

        /**
         * Destructor.
         */
        ~module() = default;

        /**
         * @brief Assignment operator.
         * @return @c *this
         */
        module& operator=(const module&) = default;

        /**
         * @brief Move assignment operator.
         * @return @c *this
         */
        module& operator=(module&&) = default;

    private:
        module_details_ref m_details;
    };

} // namespace mge::script
