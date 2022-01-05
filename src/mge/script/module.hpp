// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type.hpp"

#include <string>

namespace mge::script {

    /**
     * @brief Module
     *
     * A module wraps a namespace or alike compound of objects. Modules, like
     * namespaces. are hierarchically organized.
     */
    class MGESCRIPT_EXPORT module
    {
    public:
        /**
         * Root module.
         */
        module();
        /**
         * @brief Construct a new module object
         *
         * @param identifier module path
         */
        module(const std::string& path);
        /**
         * @brief Destroy the module object
         */
        ~module();
        /**
         * @brief Copy constructor.
         */
        module(const module&) = default;
        /**
         * @brief Move constructor.
         */
        module(module&&) = default;

        /**
         * @brief Constructor.
         * @param details module details
         */
        module(const module_details_ref& details);

        /**
         * @brief Constructor.
         * @param details module details
         */
        module(module_details_ref&& details);

        /**
         * @brief Assignment.
         *
         * @return @c *this
         */
        module& operator=(const module&) = default;

        /**
         * @brief Move assignment.
         *
         * @return @c *this
         */
        module& operator=(module&&) = default;

        /**
         * @brief Add elements to the module.
         *
         * @tparam T        type of first element
         * @tparam Args     other element types
         * @param arg0      first element
         * @param args      other elements
         *
         * @return @c *this
         */
        template <typename T, typename... Args>
        module& operator()(T&& arg0, Args&&... args)
        {
            add_member(std::forward<T>(arg0));
            return operator()(std::forward<Args>(args)...);
        }

        module& operator()() { return *this; }

        bool                is_root() const;
        mge::script::module parent() const;
        const std::string&  name() const;
        std::string         full_name() const;

        void apply(visitor& v);

        module_details& details();

    private:
        void add_member(module& m) { add_module(m); }
        void add_member(type_base& m) { add_type(m); }

        void add_module(module& m);
        void add_type(type_base& t);

        module_details_ref m_details;
    };

} // namespace mge::script