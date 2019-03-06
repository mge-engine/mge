#pragma once
#include "mge/input/dllexport.hpp"
#include "mge/core/enum_set.hpp"
#include <iosfwd>

namespace mge {

    enum class MGE_INPUT_EXPORT modifier_value
    {
        CONTROL = 0,
        SHIFT = 1,
        ALT = 2
    };

    typedef enum_set<modifier_value, (size_t)  modifier_value::ALT> modifier;

    MGE_INPUT_EXPORT std::ostream& operator <<(std::ostream& os, const modifier_value& v);

}
