// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
namespace mge {

    /**
     * @brief Casts a value to a target type, and ensures it does not overflow.
     *
     * @tparam S source type
     * @tparam T target type
     * @param value value to cast
     * @return casted value
     */
    template <typename T, typename S> inline constexpr T checked_cast(S value)
    {
        T tmp = static_cast<T>(value);
        S stmp = static_cast<S>(tmp);
        if (value != stmp) {
            MGE_THROW(mge::bad_cast) << value << " cannot be casted into "
                                     << mge::type_name<T>() << " type";
        }
        return tmp;
    }

} // namespace mge