// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/container.hpp"

namespace mge::entity {

    container::container(mge::entity::registry& r)
        : entity(r)
    {}

    container::container(mge::entity::registry& r, const char* name)
        : entity(r, name)
    {}

    container::container(const mge::entity::entity& e) noexcept
        : entity(e)
    {}

    container::~container() {}

    void container::add(const mge::entity::entity& entity)
    {
        entity.add(mge::entity::entity::child_of, *this);
    }

} // namespace mge::entity