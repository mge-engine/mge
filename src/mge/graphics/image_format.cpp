// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/image_format.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator<<(std::ostream& os, const image_format& f)
    {
        return os << std::format("{}", f);
    }

} // namespace mge