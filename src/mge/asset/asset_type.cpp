#include "mge/asset/asset_type.hpp"

namespace mge {
    asset_type::asset_type() {}

    asset_type::asset_type(std::string_view type, std::string_view subtype)
        : m_type(type)
        , m_subtype(subtype)
    {}

    std::string_view asset_type::type() const
    {
        return std::string_view(m_type.begin(), m_type.end());
    }

    std::string_view asset_type::subtype() const
    {
        return std::string_view(m_subtype.begin(), m_subtype.end());
    }

    bool asset_type::operator==(const asset_type& t) const
    {
        return m_type == t.m_type && m_subtype == t.m_subtype;
    }

    bool asset_type::operator<(const asset_type& t) const
    {
        if (m_type < t.m_type) {
            return true;
        } else if (m_type == t.m_type) {
            return m_subtype < t.m_subtype;
        } else {
            return false;
        }
    }

} // namespace mge