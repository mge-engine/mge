// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

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
         * @param path module path
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
        module& operator()(const T& arg0, const Args&... args)
        {
            add_member(arg0);
            return operator()(args...);
        }

        module& operator()() { return *this; }

        bool                is_root() const;
        mge::script::module parent() const;
        const std::string&  name() const;
        std::string         full_name() const;

        void apply(visitor& v) const;

        module_details& details();

        bool operator==(const module& m) const;
        bool operator!=(const module& m) const;

    private:
        void add_member(const module& m) { add_module(const_cast<module&>(m)); }
        void add_member(const function_base& f)
        {
            add_function(const_cast<function_base&>(f));
        }

        template <typename T, typename V> void add_member(const type<T, V>& t)
        {
            add_type(const_cast<type_base&>(static_cast<const type_base&>(t)));
        }

        void add_module(module& m);
        void add_type(type_base& t);
        void add_function(function_base& f);

        module_details_ref m_details;
    };

} // namespace mge::script

namespace std {
    template <> struct hash<mge::script::module>
    {
        size_t operator()(const mge::script::module& m) const
        {
            const std::hash<std::string> string_hash;
            return string_hash(m.name());
        }
    };
} // namespace std