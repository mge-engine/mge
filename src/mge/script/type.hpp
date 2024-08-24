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
        bool is_class() const noexcept { return false; }

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
                m_data->pod_specific().size = sizeof(T);
            }
        }

        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return true; }
        bool is_class() const noexcept { return false; }

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
                m_data->pod_specific().size = sizeof(bool);
            }
        }

        bool is_bool() const noexcept { return true; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return true; }
        bool is_class() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

    private:
        type_data_ref m_data;
    };

    template <typename T>
        requires std::is_class_v<T>
    class type<T>
    {
    public:
        type()
        {
            m_data = type_data::get(typeid(T));
            if (!m_data) {
                m_data =
                    type_data::create(typeid(T), type_data::type_kind::CLASS);
                if constexpr (std::is_same_v<T, std::string>) {
                    m_data->class_specific().is_string = true;
                } else if constexpr (std::is_same_v<T, std::wstring>) {
                    m_data->class_specific().is_wstring = true;
                }
                m_data->class_specific().size = sizeof(T);
                if (std::is_destructible_v<T>) {
                    // m_data->class_specific().destructor
                }
            }
        }

        type(const char* alias_name)
        {
            m_data = type_data::get(typeid(T));
            if (!m_data) {
                m_data = type_data::create(typeid(T),
                                           type_data::type_kind::CLASS,
                                           alias_name);
                if constexpr (std::is_same_v<T, std::string>) {
                    m_data->class_specific().is_string = true;
                } else if constexpr (std::is_same_v<T, std::wstring>) {
                    m_data->class_specific().is_wstring = true;
                }
                m_data->class_specific().size = sizeof(T);
            }
        }

        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return true; }

        const type_data_ref& data() const noexcept { return m_data; }

        std::enable_if_t<std::is_default_constructible_v<T>, type<T>&>
        constructor()
        {
            // m_data->class_specific().is_default_constructible = true;
            return *this;
        }

    private:
        type_data_ref m_data;
    };

    template <typename T>
        requires std::is_pointer_v<T>
    class type<T>
    {
    public:
        type()
        {
            m_data = type_data::get(typeid(T));
            if (!m_data) {
                m_data =
                    type_data::create(typeid(T), type_data::type_kind::POINTER);
                // plain pointee type, no const here
                type<std::remove_cv_t<std::remove_pointer_t<T>>> pointee_type;
                m_data->pointer_specific().pointee = pointee_type.data();
                m_data->pointer_specific().is_const =
                    std::is_const_v<std::remove_pointer_t<T>>;
                m_data->pointer_specific().is_volatile =
                    std::is_volatile_v<std::remove_pointer_t<T>>;
            }
        }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

    private:
        type_data_ref m_data;
    };

} // namespace mge::script