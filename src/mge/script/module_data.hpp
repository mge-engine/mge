// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>
#include <vector>

namespace mge::script {

    /**
     * @brief Module data.
     *
     * This class is the internal representation of a module.
     */
    class MGESCRIPT_EXPORT module_data
        : public std::enable_shared_from_this<module_data>
    {
    public:
        module_data() = default;

        /**
         * Constructor for module.
         *
         * This constructor is public to allow creation internally using
         * <tt>std::make_shared</tt> and should not be used directly.
         * It does not emplace the module into the parent module.
         *
         * @param parent parent module
         * @param name   module name
         */
        module_data(const module_data_ref& parent, const std::string& name)
            : m_name(name)
            , m_parent(parent)
        {}

        module_data(const module_data&) = delete;
        module_data& operator=(const module_data&) = delete;
        module_data(module_data&&) = delete;
        module_data& operator=(module_data&&) = delete;

        ~module_data() = default;

        /**
         * Root module.
         * @return root module data
         */
        static module_data_ref root();

        /**
         * Lookup or create module data. Modules are unique in the scope of
         * the system, so asking for the module for a given name will always
         * return the same module_data instance.
         *
         * @param name qualified module name
         */
        static module_data_ref get(const std::string& name);

        bool is_root() const { return m_parent.expired() && m_name.empty(); }
        const std::string& name() const { return m_name; }
        module_data_ref    parent() const { return m_parent.lock(); }

        void add(const function_data_ref& f);
        void add(const type_data_ref& t);

        const function_data& function(const char* name) const;
        const type_data&     type(const char* name) const;

    private:
        std::string                    m_name;
        module_data_weak_ref           m_parent;
        std::vector<module_data_ref>   m_modules;
        std::vector<type_data_ref>     m_types;
        std::vector<function_data_ref> m_functions;
    };

} // namespace mge::script