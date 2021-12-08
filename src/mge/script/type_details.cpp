// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"
#include "mge/script/module.hpp"

namespace mge::script {

    type_details::type_details(std::type_index index, const std::string& name)
        : m_index(index)
        , m_name(name)
    {}

    type_details::type_details(std::type_index            index,
                               const std::string&         name,
                               const type_classification& c)
        : m_index(index)
        , m_name(name)
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

    void type_details::apply(visitor& v)
    {
        v.begin(*this);
        v.end(*this);
    }

    enum_type_details::enum_type_details(std::type_index    index,
                                         const std::string& name)
        : type_details(index, name)
    {}

    enum_type_details::enum_type_details(std::type_index            index,
                                         const std::string&         name,
                                         const type_classification& c)
        : type_details(index, name, c)
    {}

    void enum_type_details::apply(visitor& v)
    {
        v.begin(*this);
        for (const auto& value : m_enum_values) {
            v.enum_value(std::get<0>(value), std::get<1>(value));
        }
        v.end(*this);
    }
} // namespace mge::script