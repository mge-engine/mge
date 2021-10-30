#include "mge/script/script_type.hpp"

namespace mge::script {
    type::type(const std::string_view name)
        : m_name(name)
    {}

    type::~type() {}

    std::string_view type::name() const { return m_name; }

} // namespace mge