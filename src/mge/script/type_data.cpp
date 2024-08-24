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
        default:
            break;
        }
    }

    type_data::~type_data() = default;

    const std::string& type_data::name() const
    {
        if (m_name.empty()) {
            m_name = mge::base_type_name(*m_type_info);
        }
        return m_name;
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

} // namespace mge::script