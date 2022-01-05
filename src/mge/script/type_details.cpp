// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/visitor.hpp"

namespace mge::script {

    class type_dictionary
    {
    public:
        type_dictionary();
        ~type_dictionary() = default;

        type_details_ref get(const std::type_index& ti)
        {
            auto it = m_types.find(ti);
            if (it == m_types.end()) {
                MGE_THROW(no_such_element)
                    << "Type details not found for:" << ti.name();
            }
            return it->second;
        }

        type_details_ref get_nothrow(const std::type_index& ti)
        {
            auto it = m_types.find(ti);
            if (it == m_types.end()) {
                return nullptr;
            }
            return it->second;
        }

        type_details_ref add(const std::type_index& ti,
                             const std::string&     name,
                             const traits&          tr)
        {
            if (tr.is_enum) {
                auto et = std::make_shared<enum_type_details>(name, ti, tr);
                m_types[ti] = et;
                return et;
            }

            return type_details_ref();
        }

    private:
        template <typename T>
        void add_pod_type_details(const char* used_name = nullptr)
        {
            auto new_type =
                std::make_shared<type_details>(mge::type_name<T>(),
                                               std::type_index(typeid(T)),
                                               traits_of<T>(),
                                               used_name);
            m_types[new_type->type_index()] = new_type;
            module_details::get("")->add_type(new_type);
        }

        std::map<std::type_index, type_details_ref> m_types;
    };

    type_dictionary::type_dictionary()
    {
        add_pod_type_details<void>();
        add_pod_type_details<signed char>();
        add_pod_type_details<unsigned char>();
        add_pod_type_details<char>();
        add_pod_type_details<short>();
        add_pod_type_details<unsigned short>();
        add_pod_type_details<int>();
        add_pod_type_details<unsigned int>();
        add_pod_type_details<long>();
        add_pod_type_details<unsigned long>();
        add_pod_type_details<int64_t>("int64_t");
        add_pod_type_details<uint64_t>("uint64_t");
        add_pod_type_details<float>();
        add_pod_type_details<double>();
        add_pod_type_details<long double>();
        add_pod_type_details<bool>();
    }

    static mge::singleton<type_dictionary> s_type_dictionary;

    type_details::type_details(const std::string&     automatic_name,
                               const std::type_index& ti,
                               const traits&          t,
                               const char*            name)
        : m_automatic_name(automatic_name)
        , m_name(name == nullptr ? "" : name)
        , m_type_index(ti)
        , m_traits(t)
    {}

    const std::string& type_details::name() const
    {
        return m_name.empty() ? m_automatic_name : m_name;
    }

    const module_details_weak_ref& type_details::module() const
    {
        return m_module;
    }

    const std::type_index& type_details::type_index() const
    {
        return m_type_index;
    }

    void type_details::set_module(const module_details_ref& m) { m_module = m; }

    type_details_ref type_details::get(const std::type_index& ti)
    {
        return s_type_dictionary->get(ti);
    }

    type_details_ref type_details::get_or_create(const std::type_index& ti,
                                                 const std::string&     name,
                                                 const traits&          tr)
    {
        auto result = s_type_dictionary->get_nothrow(ti);
        if (!result) {
            result = s_type_dictionary->add(ti, name, tr);
        }
        return result;
    }

    void type_details::apply(visitor& v)
    {
        v.start(*this);
        v.finish(*this);
    }

    enum_type_details::enum_type_details(const std::string&     name,
                                         const std::type_index& ti,
                                         const traits&          t,
                                         const char*            used_name)
        : type_details(name, ti, t, used_name)
    {}

    void enum_type_details::apply(visitor& v)
    {
        v.start(*this);
        for (const auto& value : m_values) {
            v.enum_value(std::get<0>(value), std::get<1>(value));
        }
        v.finish(*this);
    }

    void enum_type_details::enum_value(const std::string& name, int64_t value)
    {
        m_values.emplace_back(std::make_tuple(name, value));
    }

} // namespace mge::script