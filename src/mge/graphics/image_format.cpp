// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/image_format.hpp"
#include <iostream>

namespace mge {

    void image_format::format(std::format_context& context) const
    {
        std::format_to(context.out(), "{}_{}", m_format, m_type);
    }

} // namespace mge