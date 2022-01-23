// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type_details.hpp"

namespace mge::script {
    type_base::type_base() {}

    type_base& type_base::operator=(const type_base& t)
    {
        m_details = t.m_details;
        return *this;
    }

    const std::string& type_base::name() const { return m_details->name(); }

    mge::script::module type_base::module() const
    {
        return mge::script::module(m_details->module().lock());
    }

    const std::type_index& type_base::type_index() const
    {
        return m_details->type_index();
    }

    const type_details_ref& type_base::details() const { return m_details; }

    type_details_ref& type_base::details() { return m_details; }

    void type_base::init_details(const std::type_index& ti)
    {
        m_details = type_details::get(ti);
    }

    void type_base::init_enum_details(const std::type_index& index,
                                      const std::string&     name,
                                      const traits&          tr)
    {
        m_details = type_details::get_or_create(index, name, tr);
    }

    void type_base::init_class_details(const std::type_index& index,
                                       const std::string&     name,
                                       const traits&          tr)
    {
        m_details = type_details::get_or_create(index, name, tr);
    }

    void type_base::enum_value(const std::string& name, int64_t value)
    {
        std::static_pointer_cast<enum_type_details>(m_details)->enum_value(
            name,
            value);
    }

    void type_base::set_base(const type_details_ref& base_details)
    {
        std::static_pointer_cast<class_type_details>(m_details)->set_base(
            base_details);
    }

    void type_base::set_destructor(const invoke_function& dtor)
    {
        std::static_pointer_cast<class_type_details>(m_details)->set_destructor(
            dtor);
    }

    void type_base::add_constructor(const signature&       sgn,
                                    const invoke_function& ctor)
    {
        std::static_pointer_cast<class_type_details>(m_details)
            ->add_constructor(sgn, ctor);
    }

} // namespace mge::script