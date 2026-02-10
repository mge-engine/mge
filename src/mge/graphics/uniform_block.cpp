// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform_block.hpp"
#include "mge/core/memory.hpp"

namespace mge {

    uniform_block::uniform_block(size_t data_size)
        : m_data(::mge::malloc(data_size))
    {}

    uniform_block::~uniform_block()
    {
        ::mge::free(m_data);
    }

} // namespace mge