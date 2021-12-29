#include "mge/script/module_details.hpp"
#include "boost/boost_algorithm_string.hpp"

namespace mge::script {

    module_details module_details::s_root_module;

    module_details::module_details(const std::string& name)
        : m_name(name)
        , m_parent(nullptr)
    {}

    module_details::module_details()
        : m_parent(nullptr)
    {}

    module_details::~module_details()
    {
        for (const auto& [n, m] : m_modules) {
            delete m;
        }
    }

    module_details* module_details::get(const std::string& path)
    {
        if (path.empty()) {
            return &s_root_module;
        }

        std::vector<std::string> modules;
        boost::iter_split(modules, path, boost::algorithm::first_finder("::"));

        module_details* current_module = &s_root_module;
        for (const auto& modulename : modules) {
            current_module = current_module->get_or_add_module(modulename);
        }

        return current_module;
    }

    module_details* module_details::get_or_add_module(const std::string& name)
    {
        auto it = m_modules.find(name);
        if (it != m_modules.end()) {
            return it->second;
        } else {
            auto new_module = std::make_unique<module_details>(name);
            new_module->m_parent = this;
            module_details* result = new_module.get();
            m_modules[name] = new_module.get();
            new_module.reset();
            return result;
        }
    }

} // namespace mge::script