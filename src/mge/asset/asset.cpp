#include "mge/asset/asset.hpp"

namespace mge {
    size_t
    asset::size() const
    {
        return 0;
    }

    mge::input_stream_ref
    asset::data() const
    {
        mge::input_stream_ref result;
        return result;
    }

    asset operator ""_asset(const char *s, size_t sz)
    {
        return asset(std::string(s, s+sz));
    }
}