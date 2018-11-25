// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include <cstdint>
#include "mge/reflection/type.hpp"

#include <string>

namespace mge {
    namespace reflection {
        void bind_std()
        {
            type<std::string> t_std_string;
        }
    }
}
