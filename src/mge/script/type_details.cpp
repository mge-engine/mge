// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/type_details.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/module_details.hpp"

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
        template <typename T> void add_pod_type_details()
        {
            auto newp =
                std::make_unique<type_details>(mge::type_name<T>(),
                                               std::type_index(typeid(T)),
                                               traits_of<T>());
            m_types.insert(std::make_pair(newp->type_index(), newp.get()));
            try {
                module_details::get("")->add_type(newp.get());
            } catch (...) {
                m_types.erase(newp->type_index());
                throw;
            }
            newp.release();
        }

        std::map<std::type_index, type_details*> m_types;
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
        add_pod_type_details<int64_t>();
        add_pod_type_details<uint64_t>();
        add_pod_type_details<float>();
        add_pod_type_details<double>();
        add_pod_type_details<long double>();
    }

    static mge::singleton<type_dictionary> s_type_dictionary;

    type_details::type_details(const std::string&     name,
                               const std::type_index& ti,
                               const traits&          t)
        : m_name(name)
        , m_type_index(ti)
        , m_traits(t)
    {}

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