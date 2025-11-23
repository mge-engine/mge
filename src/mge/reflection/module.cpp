// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/module.hpp"
#include "mge/core/trace.hpp"
#include "mge/reflection/module_details.hpp"

namespace mge {
    MGE_DEFINE_TRACE(REFLECTION);
}

namespace mge::reflection {

    module::module() :m_details(module_details::root()) {}

    module::module(const std::string_view name) :m_details(
        module_details::get(name))
    {}

    bool module::is_root() const
    {
        return m_details->is_root();
    }

    std::string_view module::name() const
    {
        return m_details->name();
    }

    std::string module::full_name() const
    {
        return m_details->full_name();
    }

    mge::reflection::module module::parent() const
    {
        return mge::reflection::module(m_details->parent());
    }

    void module::add_details(const type_details_ref& details)
    {
        m_details->add(details);
    }

    void module::add_details(const function_details_ref& details)
    {
        m_details->add(details);
    }

} // namespace mge::reflection
