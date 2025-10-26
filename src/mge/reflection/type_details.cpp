// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/type_details.hpp"
#include "mge/core/singleton.hpp"

#include <unordered_map>

namespace mge::reflection {

    class type_details_dictionary
    {
    public:
        const type_details_ref& get(const type_identifier& id)
        {
            auto it = m_type_details_map.find(id);
            if (it != m_type_details_map.end()) {
                return it->second;
            }
            return m_default_details;
        }

        const type_details_ref& put(const type_identifier&  id,
                                    const type_details_ref& details)
        {
            auto it = m_type_details_map.find(id);
            if (it == m_type_details_map.end()) {
                m_type_details_map[id] = details;
                it = m_type_details_map.find(id);
            }
            return it->second;
        }

    private:
        std::unordered_map<type_identifier, type_details_ref>
                         m_type_details_map;
        type_details_ref m_default_details;
    };

    singleton<type_details_dictionary> s_type_details_dictionary;

    type_details::type_details() {}

    type_details::~type_details() {}

    const type_details_ref& type_details::get(const type_identifier& id)
    {
        return s_type_details_dictionary->get(id);
    }

    const type_details_ref& type_details::put(const type_identifier&  id,
                                              const type_details_ref& details)
    {
        return s_type_details_dictionary->put(id, details);
    }

} // namespace mge::reflection
