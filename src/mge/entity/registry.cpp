// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/entity/registry.hpp"

namespace mge::entity {

    std::atomic<registry_id_type> registry::s_registry_id_sequence(1);

    registry::registry()
        : m_id_sequence(1)
        , m_registry_id(s_registry_id_sequence++)
    {}

    registry::~registry() {}

} // namespace mge::entity
