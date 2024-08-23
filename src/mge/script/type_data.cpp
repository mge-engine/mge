#include "mge/script/type_data.hpp"
#include "mge/core/singleton.hpp"

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

    type_data_ref type_data::create(const std::type_info& ti)
    {
        auto td = std::make_shared<type_data>(ti);
        s_all_types->put(std::type_index(ti), td);
        return td;
    }

    type_data::type_data(const std::type_info& ti)
        : m_type_info(&ti)
    {}

    type_data::~type_data() = default;

    const std::string& type_data::name() const
    {
        if (m_name.empty()) {
            m_name = m_type_info->name();
        }
        return m_name;
    }

} // namespace mge::script