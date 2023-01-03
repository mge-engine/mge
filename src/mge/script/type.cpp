// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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
        m_details = type_details::get_first_match(ti);
    }

    void type_base::init_enum_details(const std::type_index& index,
                                      const std::string&     name,
                                      const traits&          tr,
                                      size_t                 size)
    {
        m_details = type_details::create(index, name, tr, size);
    }

    void type_base::init_class_details(const std::type_index& index,
                                       const std::string&     name,
                                       const traits&          tr,
                                       size_t                 size)
    {
        m_details = type_details::create(index, name, tr, size);
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

    void type_base::set_destructor(const invoke_function& delete_ptr,
                                   const invoke_function& delete_shared_ptr)
    {
        std::static_pointer_cast<class_type_details>(m_details)->set_destructor(
            delete_ptr,
            delete_shared_ptr);
    }

    void type_base::add_constructor(const signature&       sgn,
                                    const invoke_function& ctor,
                                    const invoke_function& make_shared)
    {
        std::static_pointer_cast<class_type_details>(m_details)
            ->add_constructor(sgn, ctor, make_shared);
    }

    void type_base::add_method(const std::string&     name,
                               const std::type_index& return_type,
                               const signature&       sgn,
                               const invoke_function& invoke)
    {
        std::static_pointer_cast<class_type_details>(m_details)
            ->add_method(name, return_type, sgn, invoke);
    }

    void type_base::add_static_method(const std::string&     name,
                                      const std::type_index& return_type,
                                      const signature&       sgn,
                                      const invoke_function& invoke)
    {
        std::static_pointer_cast<class_type_details>(m_details)
            ->add_static_method(name, return_type, sgn, invoke);
    }

    void type_base::add_field(const std::string&     name,
                              const type_base&       type,
                              const invoke_function& getter)
    {
        std::static_pointer_cast<class_type_details>(m_details)->add_field(
            name,
            type.m_details,
            getter);
    }

    void type_base::add_field(const std::string&     name,
                              const type_base&       type,
                              const invoke_function& getter,
                              const invoke_function& setter)
    {
        std::static_pointer_cast<class_type_details>(m_details)
            ->add_field(name, type.m_details, getter, setter);
    }

    void type_base::add_member(type_base& t)
    {
        std::static_pointer_cast<class_type_details>(m_details)->add_type(
            t.details());
    }

} // namespace mge::script