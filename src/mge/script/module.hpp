// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/sfinae.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/function.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type.hpp"
#include "mge/script/visitor.hpp"
#include <map>
#include <string>

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
        module(const std::string& identifier);

        /**
         * @brief Copy constructor.
         */
        module(const module&) = default;

        /**
         * Move constructor.
         */
        module(module&&) = default;

        /**
         * Create module with existing details.
         *
         * @param details module details
         */
        module(const module_details_ref& details);

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

        template <typename T, typename... Args>
        module& operator()(T&& arg0, Args&&... args)
        {
            add_child(std::forward<T>(arg0));
            return operator()(std::forward<Args>(args)...);
        }

        /**
         * @brief Return whether this module is root module.
         *
         * @return true if it is the root module
         */
        bool is_root() const;

        /**
         * @brief Get parent module.
         *
         * @return module parent module
         */
        mge::script::module parent() const;

        /**
         * @brief Module name.
         *
         * @return name
         */
        const std::string& name() const;

        /**
         * @brief Full module name.
         *
         * @return full name, components separated by '::'
         */
        std::string full_name() const;

        /**
         * @brief Apply visitor.
         *
         * @param v visitor
         */
        void apply(visitor& v) const;

    private:
        friend class type_details;

        void add_child(module&& m);

        template <typename T> void add_child(type<T, void>&& t)
        {
            add_type(t.details());
        }

        template <typename T> void add_child(const type<T, void>& t)
        {
            add_type(t.details());
        }

        template <typename T> void add_child(variable<T>&& v)
        {
            add_variable(v.details());
        }

        template <typename R, typename... Args>
        void add_child(mge::script::function<R, Args...>&& f)
        {
            add_function(f.details());
        }

        template <typename R, typename... Args>
        void add_child(const mge::script::function<R, Args...>& f)
        {
            add_function(f.details());
        }

        void add_type(const type_details_ref& details);
        void add_variable(const variable_details_ref& details);
        void add_function(const function_details_ref& details);

        inline module& operator()() { return *this; }

        module_details_ref m_details;
    };

} // namespace mge::script
