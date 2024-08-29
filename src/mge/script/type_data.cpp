// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_data.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"

#include <map>
#include <typeindex>

namespace mge::script {
    class type_dictionary
    {
    public:
        type_dictionary() = default;
        ~type_dictionary() = default;

        type_data_ref get(const std::type_index& ti)
        {
            type_data_ref result;

            auto i = m_types.find(ti);
            if (i != m_types.end()) {
                result = i->second;
            }

            return result;
        }

        void put(const std::type_index& ti, const type_data_ref& td)
        {
            m_types[ti] = td;
        }

        std::map<std::type_index, type_data_ref> m_types;
    };

    mge::singleton<type_dictionary> s_all_types;

    type_data_ref type_data::get(const std::type_info& ti)
    {
        return s_all_types->get(std::type_index(ti));
    }

    type_data_ref type_data::create(const std::type_info& ti,
                                    type_data::type_kind  kind)
    {
        auto td = std::make_shared<type_data>(ti, kind);
        s_all_types->put(std::type_index(ti), td);
        return td;
    }

    type_data::type_data(const std::type_info& ti, type_data::type_kind kind)
        : m_type_info(&ti)
    {
        switch (kind) {
        case type_kind::ENUM:
            m_details = enum_details();
            break;
        case type_kind::CLASS:
            m_details = class_details();
            break;
        case type_kind::POD:
            m_details = pod_details();
            break;
        case type_kind::POINTER:
            m_details = pointer_details();
            break;
        case type_kind::REFERENCE:
            m_details = reference_details();
            break;
        case type_kind::RVALUE_REFERENCE:
            m_details = rvalue_reference_details();
            break;
        case type_kind::VOID:
            m_details = void_details();
            break;
        default:
            break;
        }
    }

    type_data::~type_data() = default;

    std::string type_data::name() const { return ""; }

    type_data::enum_details& type_data::enum_specific()
    {
        if (m_details.index() != 1) {
            MGE_THROW(illegal_state) << "Type is not an enum";
        }
        return std::get<enum_details>(m_details);
    }

    type_data::class_details& type_data::class_specific()
    {
        if (m_details.index() != 2) {
            MGE_THROW(illegal_state) << "Type is not a class";
        }
        return std::get<class_details>(m_details);
    }

    type_data::pod_details& type_data::pod_specific()
    {
        if (m_details.index() != 3) {
            MGE_THROW(illegal_state) << "Type is not a pod type";
        }
        return std::get<pod_details>(m_details);
    }

    type_data::pointer_details& type_data::pointer_specific()
    {
        if (m_details.index() != 4) {
            MGE_THROW(illegal_state) << "Type is not a pointer";
        }
        return std::get<pointer_details>(m_details);
    }

    type_data::reference_details& type_data::reference_specific()
    {
        if (m_details.index() != 5) {
            MGE_THROW(illegal_state) << "Type is not a reference";
        }
        return std::get<reference_details>(m_details);
    }

    type_data::rvalue_reference_details& type_data::rvalue_reference_specific()
    {
        if (m_details.index() != 6) {
            MGE_THROW(illegal_state) << "Type is not a rvalue reference";
        }
        return std::get<rvalue_reference_details>(m_details);
    }

    type_data::void_details& type_data::void_specific()
    {
        if (m_details.index() != 7) {
            MGE_THROW(illegal_state) << "Type is not void";
        }
        return std::get<void_details>(m_details);
    }

    const type_data::enum_details& type_data::enum_specific() const
    {
        if (m_details.index() != 1) {
            MGE_THROW(illegal_state) << "Type is not an enum";
        }
        return std::get<enum_details>(m_details);
    }

    const type_data::class_details& type_data::class_specific() const
    {
        if (m_details.index() != 2) {
            MGE_THROW(illegal_state) << "Type is not a class";
        }
        return std::get<class_details>(m_details);
    }

    const type_data::pod_details& type_data::pod_specific() const
    {
        if (m_details.index() != 3) {
            MGE_THROW(illegal_state) << "Type is not a pod type";
        }
        return std::get<pod_details>(m_details);
    }

    const type_data::pointer_details& type_data::pointer_specific() const
    {
        if (m_details.index() != 4) {
            MGE_THROW(illegal_state) << "Type is not a pointer";
        }
        return std::get<pointer_details>(m_details);
    }

    const type_data::reference_details& type_data::reference_specific() const
    {
        if (m_details.index() != 5) {
            MGE_THROW(illegal_state) << "Type is not a reference";
        }
        return std::get<reference_details>(m_details);
    }

    const type_data::rvalue_reference_details&
    type_data::rvalue_reference_specific() const
    {
        if (m_details.index() != 6) {
            MGE_THROW(illegal_state) << "Type is not a rvalue reference";
        }
        return std::get<rvalue_reference_details>(m_details);
    }

    const type_data::void_details& type_data::void_specific() const
    {
        if (m_details.index() != 7) {
            MGE_THROW(illegal_state) << "Type is not void";
        }
        return std::get<void_details>(m_details);
    }

    bool type_data::is_pod() const { return m_details.index() == 3; }

    bool type_data::is_enum() const { return m_details.index() == 1; }

    bool type_data::is_class() const { return m_details.index() == 2; }

    bool type_data::is_pointer() const { return m_details.index() == 4; }

    bool type_data::is_reference() const { return m_details.index() == 5; }

    bool type_data::is_rvalue_reference() const
    {
        return m_details.index() == 6;
    }

    bool type_data::is_void() const { return m_details.index() == 7; }

    bool type_data::is_string() const
    {
        return m_details.index() == 2 &&
               std::get<class_details>(m_details).is_string;
    }

    bool type_data::is_wstring() const
    {
        return m_details.index() == 2 &&
               std::get<class_details>(m_details).is_wstring;
    }

    bool type_data::is_const() const
    {
        switch (m_details.index()) {
        case 4:
            return std::get<pointer_details>(m_details).is_const;
        case 5:
            return std::get<reference_details>(m_details).is_const;
        case 6:
            return std::get<rvalue_reference_details>(m_details).is_const;
        default:
            return false;
        }
    }

    bool type_data::is_volatile() const
    {
        switch (m_details.index()) {
        case 4:
            return std::get<pointer_details>(m_details).is_volatile;
        case 5:
            return std::get<reference_details>(m_details).is_volatile;
        case 6:
            return std::get<rvalue_reference_details>(m_details).is_volatile;
        default:
            return false;
        }
    }

    bool type_data::exposed_directly() const { return !m_module.expired(); }

} // namespace mge::script