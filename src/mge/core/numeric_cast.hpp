#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"

namespace mge {

    template <typename T, typename S>
    inline T numeric_cast(const S src)
    {
        T tmp = static_cast<T>(src);
        S stmp = static_cast<S>(tmp);
        if(src != stmp) {
            MGE_THROW(mge::out_of_range)
                << src << " cannot be casted into "
                << mge::type_name<T>() << " type";
        }
        return tmp;
    }
}
