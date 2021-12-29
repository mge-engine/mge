#pragma once

#include "mge/script/dllexport.hpp"
#include "mge/script/module.hpp"

#include <memory>
#include <string>
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

    private:
        module_details* get_or_add_module(const std::string& name);

        using module_map = std::unordered_map<std::string, module_details*>;

        std::string     m_name;
        module_details* m_parent;
        module_map      m_modules;

        static module_details s_root_module;
    };

} // namespace mge::script