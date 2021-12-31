#include "mge/script/type_details.hpp"

namespace mge::script {
    const std::string& type_details::name() const { return m_name; }

    module_details* type_details::module() const { return m_module; }

    const std::type_index& type_details::type_index() const
    {
        return m_type_index;
    }

    void type_details::set_module(module_details* m) { m_module = m; }
} // namespace mge::script