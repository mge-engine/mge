// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/entity.hpp"

namespace mge::entity {
    entity::entity()
        : m_entity(0)
        , m_flags(0)
        , m_refcount(0)
    {}

} // namespace mge::entity