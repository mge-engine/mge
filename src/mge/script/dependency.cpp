#include "mge/script/dependency.hpp"
#include "mge/script/function_data.hpp"

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
} // namespace mge::script