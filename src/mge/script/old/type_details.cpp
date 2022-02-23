// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}

namespace mge::script {

    static context_function s_empty_context_function;

    type_details::type_details(std::type_index index, const std::string& name)
        : m_index(index)
        , m_name(name)
        , m_size(0)
    {}

    type_details::type_details(std::type_index            index,
                               const std::string&         name,
                               const type_classification& c,
                               size_t                     sz)
        : m_index(index)
        , m_name(name)
        , m_size(sz)
        , m_type_class(c)
    {}

    std::type_index type_details::type_index() const { return m_index; }

    const std::string& type_details::name() const { return m_name; }

    const type_classification& type_details::type_class() const
    {
        return m_type_class;
    }

    mge::script::module type_details::module() const
    {
        if (m_module.expired()) {
            MGE_THROW(illegal_state)
                << "Type '" << name() << "' is incomplete, no module set";
        }
        auto m = m_module.lock();
        return mge::script::module(m);
    }

    std::string type_details::full_name() const
    {
        return module().full_name() + name();
    }

    size_t type_details::type_size() const { return m_size; }

    const std::string& type_details::alias() const { return m_alias_name; }

    const context_function& type_details::destructor() const
    {
        return s_empty_context_function;
    }

    void type_details::apply(visitor& v)
    {
        v.begin(*this);
        v.end(*this);
    }

    void type_details::set_alias_name(const std::string& alias)
    {
        MGE_DEBUG_TRACE(SCRIPT)
            << "Set alias for '" << name() << "' to: " << alias;
        m_alias_name = alias;
    }

    enum_type_details::enum_type_details(std::type_index    index,
                                         const std::string& name)
        : type_details(index, name)
    {}

    enum_type_details::enum_type_details(std::type_index            index,
                                         const std::string&         name,
                                         const type_classification& c,
                                         size_t                     size)
        : type_details(index, name, c, size)
    {}

    void enum_type_details::apply(visitor& v)
    {
        v.begin(*this);
        for (const auto& value : m_enum_values) {
            v.enum_value(std::get<0>(value), std::get<1>(value));
        }
        v.end(*this);
    }

    class_type_details::class_type_details(std::type_index    index,
                                           const std::string& name)
        : type_details(index, name)
    {}

    class_type_details::class_type_details(std::type_index            index,
                                           const std::string&         name,
                                           const type_classification& c,
                                           size_t                     size)
        : type_details(index, name, c, size)
    {}

    void class_type_details::add_field(const std::string& name,
                                       std::type_index    type,
                                       context_function&& getter,
                                       context_function&& setter)
    {
        m_fields.emplace_back(name, type, std::move(getter), std::move(setter));
    }

    void class_type_details::use_destructor(const context_function& dtor)
    {
        m_destructor = dtor;
    }

    void class_type_details::add_constructor(const context_function& ctor)
    {
        signature empty_signature;
        m_constructors[empty_signature] = ctor;
    }

    void class_type_details::add_constructor(const signature&        sig,
                                             const context_function& ctor)
    {
        m_constructors[sig] = ctor;
    }

    void class_type_details::apply(visitor& v)
    {
        v.begin(*this);
        for (const auto& f : m_fields) {
            v.field(f);
        }
        for (const auto& [s, cf] : m_constructors) {
            v.constructor(s, cf);
        }
        v.end(*this);
    }

    const context_function& class_type_details::destructor() const
    {
        return m_destructor;
    }

} // namespace mge::script