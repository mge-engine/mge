// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"
#include "mge/script/module.hpp"

namespace mge::script {
    type_details::type_details(const std::string& name)
        : m_name(name)
    {}

    const std::string& type_details::name() const { return m_name; }

    mge::script::module type_details::module() const
    {
        if (m_module.expired()) {
            MGE_THROW(illegal_state)
                << "Type '" << name() << "' is incomplete, no module set";
        }
        auto m = m_module.lock();
        return mge::script::module(m);
    }
} // namespace mge::script