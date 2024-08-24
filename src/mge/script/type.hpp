#pragma once
#include "mge/core/enum.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_data.hpp"

#include <type_traits>

namespace mge::script {

    namespace {} // namespace

    // Simple Types
    // - integers
    // - floating point
    // - boolean
    // - string, wstring, ...
    // - enumerations
    // - void
    // Object Types
    // - classes
    // - shared_ptr<T>
    // Container Types
    // - vector<T>
    // - map<K,V>
    // - set<T>
    // - list<T>
    // - array<T,N>
    // - tuple<T...>
    // Function Type
    // - std::function
    // - function pointers ?
    // Uncreatable Types
    // - references to simple types, objects or containers
    // - pointers to simple types, objects or containers
    // Incompatible Types
    // - unique_ptr<T>
    // - containers of classes

    template <> class type<void>
    {};

    template <typename T> class type;

    template <typename T>
        requires std::is_enum_v<T>
    class type<T>
    {
    public:
        type()
        {
            m_data = type_data::get(typeid(T));
            if (!m_data) {
                m_data =
                    type_data::create(typeid(T), type_data::type_kind::ENUM);
                m_data->enum_specific().underlying_type =
                    type_data::get(typeid(mge::underlying_type_t<T>));
                for (auto& v : mge::enum_entries<T>()) {
                    m_data->enum_specific().values.emplace_back(v.first,
                                                                v.second);
                }
            }
        }

        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return true; }
        bool is_pod() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

    private:
        type_data_ref m_data;
    };

    template <typename T>
        requires std::is_integral_v<T> || std::is_floating_point_v<T>
    class type<T>
    {
    public:
        type()
        {
            m_data = type_data::get(typeid(T));
            if (!m_data) {
                m_data =
                    type_data::create(typeid(T), type_data::type_kind::POD);
            }
        }

        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return true; }

        const type_data_ref& data() const noexcept { return m_data; }

    private:
        type_data_ref m_data;
    };

    template <> class type<bool>
    {
    public:
        type()
        {
            m_data = type_data::get(typeid(bool));
            if (!m_data) {
                m_data =
                    type_data::create(typeid(bool), type_data::type_kind::POD);
            }
        }

        bool is_bool() const noexcept { return true; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return true; }

        const type_data_ref& data() const noexcept { return m_data; }

    private:
        type_data_ref m_data;
    };
} // namespace mge::script