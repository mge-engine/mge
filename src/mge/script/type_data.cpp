// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_data.hpp"

#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/type_name.hpp"

#include <map>
#include <ranges>
#include <typeindex>

namespace mge {
    MGE_USE_TRACE(SCRIPT);
}
namespace mge::script {
    class type_dictionary
    {
    public:
        type_dictionary() = default;
        ~type_dictionary() = default;

        type_data_ref get(const type_identifier& k)
        {
            type_data_ref result;

            auto i = m_types.find(k);
            if (i != m_types.end()) {
                result = i->second;
            }

            return result;
        }

        auto all()
        {
            return std::ranges::subrange(m_types.begin(), m_types.end());
        }

        void put(const type_data_ref& td)
        {
            m_types[td->identifier()] = td;
        }

        std::map<type_identifier, type_data_ref> m_types;
    };

    mge::singleton<type_dictionary> s_all_types;

    type_data_ref type_data::get(const type_identifier& id)
    {
        return s_all_types->get(id);
    }

    type_data::range_type type_data::all()
    {
        return s_all_types->all();
    }

    type_data_ref type_data::create(const std::type_info&  ti,
                                    const type_identifier& id)
    {
        auto td = std::make_shared<type_data>(ti, id);
        s_all_types->put(td);
        return td;
    }

    type_data::type_data(const std::type_info& ti, const type_identifier& id)
        : m_type_info(&ti)
        , m_identifier(id)
    {
        MGE_DEBUG_TRACE(SCRIPT) << "Creating type data for '" << name() << "'";
        switch (m_identifier.kind()) {
        case type_identifier::TYPE_ENUM:
            m_details = enum_details();
            break;
        case type_identifier::TYPE_CLASS:
            m_details = class_details();
            break;
        case type_identifier::TYPE_POD:
            m_details = pod_details();
            break;
        case type_identifier::TYPE_POINTER:
            m_details = pointer_details();
            break;
        case type_identifier::TYPE_REFERENCE:
            m_details = reference_details();
            break;
        case type_identifier::TYPE_RVALUE_REFERENCE:
            m_details = rvalue_reference_details();
            break;
        case type_identifier::TYPE_VOID:
            m_details = void_details();
            break;
        default:
            break;
        }
    }

    type_data::~type_data() = default;

    std::string type_data::name() const
    {
        // TODO: Use name in type identifier

        // alias has type identifier of aliased type
        // so "pure name" does not change
        std::string result;
        if (m_identifier.is_const()) {
            result += "const ";
        }
        if (m_identifier.is_volatile()) {
            result += "volatile ";
        }
        result += type_name(*m_type_info);
        auto kind = m_identifier.kind();
        switch (kind) {
        case type_identifier::TYPE_POINTER:
            break;
        case type_identifier::TYPE_REFERENCE:
            result += "&";
            break;
        case type_identifier::TYPE_RVALUE_REFERENCE:
            result += "&&";
            break;
        default:
            break;
        }
        return result;
    }

    std::string type_data::exposed_name() const
    {
        if (has_alias()) {
            auto an = alias_name();
            return std::string(an.begin(), an.end());
        } else {
            if (is_enum()) {
                return enum_specific().name;
            } else if (is_class()) {
                if (!class_specific().name.empty()) {
                    return class_specific().name;
                }
            }
            return generic_name();
        }
    }

    std::string type_data::generic_name() const
    {
        std::stringstream ss;
        ss << "type_" << static_cast<const void*>(this);
        return ss.str();
    }

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

    bool type_data::is_pod() const
    {
        return m_details.index() == 3;
    }

    bool type_data::is_enum() const
    {
        return m_details.index() == 1;
    }

    bool type_data::is_class() const
    {
        return m_details.index() == 2;
    }

    bool type_data::is_pointer() const
    {
        return m_details.index() == 4;
    }

    bool type_data::is_reference() const
    {
        return m_details.index() == 5;
    }

    bool type_data::is_rvalue_reference() const
    {
        return m_details.index() == 6;
    }

    bool type_data::is_void() const
    {
        return m_details.index() == 7;
    }

    bool type_data::is_callable() const
    {
        return m_details.index() == 2 &&
               std::get<class_details>(m_details).is_callable;
    }

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
        return m_identifier.is_const();
    }

    bool type_data::is_volatile() const
    {
        return m_identifier.is_volatile();
    }

    bool type_data::has_alias() const
    {
        return m_alias_name.has_value();
    }

    bool type_data::exposed_directly() const
    {
        return !m_module.expired();
    }

    std::string_view type_data::alias_name() const
    {
        if (m_alias_name.has_value()) {
            return *m_alias_name;
        } else {
            return std::string_view();
        }
    }

} // namespace mge::script