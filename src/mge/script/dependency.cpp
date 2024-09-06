#include "mge/script/dependency.hpp"
#include "mge/core/overloaded.hpp"
#include "mge/script/function_data.hpp"
#include "mge/script/module_data.hpp"

namespace mge::script {
    bool dependency::operator<(const dependency& other) const
    {
        if (m_data.index() < other.m_data.index()) {
            return true;
        } else if (m_data.index() > other.m_data.index()) {
            return false;
        } else {
            return m_data < other.m_data;
        }
    }

    std::ostream& operator<<(std::ostream& os, const dependency& d)
    {
        std::visit(overloaded{
                       [&os](const std::monostate&) { os << "none"; },
                       [&os](const module_data_ref& m) { os << m->name(); },
                       [&os](const function_data_ref& f) { os << f->name(); },
                       [&os](const type_data_ref& t) { os << t->name(); },
                   },
                   d.data());
        return os;
    }

} // namespace mge::script