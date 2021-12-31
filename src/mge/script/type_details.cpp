#include "mge/script/type_details.hpp"

namespace mge::script {
    const std::string& type_details::name() const { return m_name; }

    module_details* type_details::module() const { return m_module; }
} // namespace mge::script