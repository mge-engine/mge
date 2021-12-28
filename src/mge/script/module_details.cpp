#include "mge/script/module_details.hpp"

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

        return nullptr;
    }

} // namespace mge::script