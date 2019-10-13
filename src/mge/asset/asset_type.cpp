#include "mge/asset/asset_type.hpp"

namespace mge {
    const asset_type asset_type::UNKNOWN("unknown", "unknown");

    std::ostream& operator <<(std::ostream& os, const asset_type& t)
    {
        return os << t.type() << "/" << t.subtype();
    }
}