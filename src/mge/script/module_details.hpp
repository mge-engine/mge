#pragma once

#include "mge/script/dllexport.hpp"
#include "mge/script/module.hpp"

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace mge::script {

    /**
     * @brief Detail information of module.
     */
    class MGESCRIPT_EXPORT module_details
    {
    public:
        /**
         * @brief Constructor.
         *
         * @param name local module name
         */
        module_details(const std::string& name);

        /**
         * @brief Construct root module details.
         */
        module_details();

        /**
         * @brief Destructor.
         */
        ~module_details();

        /**
         * @brief Get instance by name.
         *
         * @param name module name
         * @return module details instance
         */
        static module_details* get(const std::string& path);

        bool               is_root() const { return m_parent == nullptr; }
        module_details*    parent() const { return m_parent; }
        const std::string& name() const { return m_name; }
        std::string        full_name() const;

        void apply(visitor& v);

        void add_module(module_details* m);
        void add_type(type_base& t);

    private:
        module_details* get_or_add_module(const std::string& name);

        using module_map = std::unordered_map<std::string, module_details*>;
        using type_map = std::unordered_map<std::type_index, type_details*>;

        std::string     m_name;
        module_details* m_parent;
        module_map      m_modules;
        type_map        m_types;

        static module_details s_root_module;
    };

} // namespace mge::script