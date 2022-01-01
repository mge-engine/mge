// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/core/stdasserts.hpp"
#include "mge/script/module_details.hpp"

namespace mge::script {

    module::module()
        : m_details(module_details::get(""))
    {}

    module::module(const std::string& path)
        : m_details(module_details::get(path))
    {}

    module::module(module_details* details)
        : m_details(details)
    {
        MGE_ASSERT_ARG_NOT_NULL(details);
    }

    module::~module() {}

    bool module::is_root() const { return m_details->is_root(); }

    mge::script::module module::parent() const
    {
        if (is_root()) {
            MGE_THROW(mge::illegal_state) << "Root module has no parent";
        }
        return mge::script::module(m_details->parent());
    }

    const std::string& module::name() const { return m_details->name(); }

    std::string module::full_name() const { return m_details->full_name(); }

    void module::apply(visitor& v) { m_details->apply(v); }

    void module::add_module(module& m) { m_details->add_module(m.m_details); }

    void module::add_type(type_base& t) { m_details->add_type(t); }

} // namespace mge::script