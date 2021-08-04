// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/swap_chain.hpp"
namespace mge {
    swap_chain::swap_chain(render_context& context)
        : context_object(context)
    {}

    swap_chain::~swap_chain() {}
} // namespace mge