// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/core/enum_set.hpp"
#include "mge/input/dllexport.hpp"
#include <iosfwd>

namespace mge {

    /**
     * @brief Keyboard modifier.
     */
    enum class MGEINPUT_EXPORT modifier_value
    {
        CONTROL = 0, //!< Control modifier key.
        SHIFT   = 1, //!< Shift modifier key.
        ALT     = 2  //!< Alt modifier key.
    };

    /**
     * @brief Modifier type, set of modifier keys pressed at the same time.
     */
    using modifier = enum_set<modifier_value, modifier_value::ALT>;

    /**
     * @brief Stream output operator.
     *
     * @param os    output stream
     * @param v     modifier value
     * @return @c os
     */
    MGEINPUT_EXPORT std::ostream &operator<<(std::ostream &        os,
                                             const modifier_value &v);

} // namespace mge
