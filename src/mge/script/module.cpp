// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/core/unprotect_constructor.hpp"
#include "mge/script/type.hpp"
#include "mge/script/visitor.hpp"

namespace mge::script {
    module::module(std::string_view name)
        : m_name(name)
    {}

    module::~module() {}

    module_ref module::parent() const
    {
        std::shared_ptr<module> res(m_parent);
        return res;
    }

    std::string_view module::name() const { return m_name; }

    std::string module::full_name() const
    {
        if (is_root()) {
            return "";
        } else {
            return parent()->full_name() + "::" + m_name;
        }
    }

    bool module::is_root() const { return m_name.empty(); }

    module_ref module::root()
    {
        static module_ref s_root =
            std::make_shared<unprotect_constructor<module>>("");
        return s_root;
    }

    void module::apply(visitor& v)
    {
        v.module_begin(*this);
        for (const auto& [key, val] : m_types) {
            val->apply(v);
        }
        for (const auto& [key, val] : m_children) {
            val->apply(v);
        }
        v.module_end(*this);
    }

} // namespace mge::script