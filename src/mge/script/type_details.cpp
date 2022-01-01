// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
namespace mge::script {

    class type_dictionary
    {
    public:
        type_dictionary();
        ~type_dictionary() = default;

        type_details* get(const std::type_index& ti)
        {
            auto it = m_types.find(ti);
            if (it == m_types.end()) {
                MGE_THROW(no_such_element)
                    << "Type details not found for:" << ti.name();
            }
            return it->second;
        }

    private:
        template <typename T> void add_details() { return; }

        std::map<std::type_index, type_details*> m_types;
    };

    type_dictionary::type_dictionary() { add_details<void>(); }

    static mge::singleton<type_dictionary> s_type_dictionary;

    const std::string& type_details::name() const { return m_name; }

    module_details* type_details::module() const { return m_module; }

    const std::type_index& type_details::type_index() const
    {
        return m_type_index;
    }

    void type_details::set_module(module_details* m) { m_module = m; }

    type_details* type_details::get(const std::type_index& ti)
    {
        return s_type_dictionary->get(ti);
    }
} // namespace mge::script