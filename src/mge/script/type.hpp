// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/callable.hpp"
#include "mge/core/enum.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/proxy.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_data.hpp"

#include <type_traits>

namespace mge::script {

    // POD Types
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

    template <typename T> class type;

    template <> class type<void>
    {
    public:
        type()
        {
            type_identifier id = make_type_identifier<void>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(void), id);
            }
        }
        bool is_void() const noexcept { return true; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return false; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        type_data_ref data() const noexcept { return m_data; }

    private:
        type_data_ref m_data;
    };

    template <typename T>
        requires std::is_enum_v<T>
    class type<T>
    {
    public:
        type()
        {
            type_identifier id = make_type_identifier<T>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(T), id);
                type_identifier underlying_id =
                    make_type_identifier<mge::underlying_type_t<T>>();
                m_data->enum_specific().underlying_type =
                    type_data::get(underlying_id);
                for (auto& v : mge::enum_entries<T>()) {
                    m_data->enum_specific().values.emplace_back(
                        static_cast<int64_t>(v.first),
                        v.second);
                }
            }
        }

        bool is_void() const noexcept { return false; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return true; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return false; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool directly_exposed() const noexcept
        {
            return m_data->directly_exposed();
        }

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
            type_identifier id = make_type_identifier<T>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(T), id);
                m_data->pod_specific().size = sizeof(T);
            }
        }

        bool is_void() const noexcept { return false; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return true; }
        bool is_class() const noexcept { return false; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool directly_exposed() const noexcept
        {
            return m_data->directly_exposed();
        }

    private:
        type_data_ref m_data;
    };

    template <> class type<bool>
    {
    public:
        type()
        {
            type_identifier id = make_type_identifier<bool>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(bool), id);
                m_data->pod_specific().size = sizeof(bool);
            }
        }

        bool is_bool() const noexcept { return true; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return true; }
        bool is_class() const noexcept { return false; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool exposed_directly() const noexcept
        {
            return m_data->exposed_directly();
        }

    private:
        type_data_ref m_data;
    };

    template <typename T>
        requires(std::is_class_v<T> && !mge::is_callable_v<T>)
    class type<T>
    {
    public:
        type()
        {
            type_identifier id = make_type_identifier<T>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(T), id);
                initialize();
            }
        }

        type(const char* alias_name)
        {
            type_identifier id = make_type_identifier<T>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(T), id, alias_name);
                initialize();
            }
        }

        bool is_void() const noexcept { return false; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return true; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool directly_exposed() const noexcept
        {
            return m_data->directly_exposed();
        }

        template <typename... Args> type<T>& constructor()
        {
            if constexpr (std::is_abstract_v<T>) {
                MGE_THROW(illegal_state)
                    << "Cannot create constructor for abstract class";
            }
            type_data::call_signature sig = {make_type_identifier<Args>()...};

            m_data->class_specific().constructors.emplace_back(
                sig,
                [](call_context& ctx) {
                    T*     obj = static_cast<T*>(ctx.get_this());
                    size_t index{0};
                    new (obj) T(ctx.get_parameter<Args>(index++)...);
                });

            return *this;
        }

        template <typename F> type<T>& field(const char* name, F T::*field)
        {
            type<F> field_type;
            if constexpr (std::is_const_v<F>) {
                m_data->class_specific().fields.emplace_back(
                    name,
                    field_type.data(),
                    [field](call_context& ctx) {
                        T* obj = static_cast<T*>(ctx.get_this());
                        ctx.store_result(obj->*field);
                    },
                    nullptr);
            } else {
                m_data->class_specific().fields.emplace_back(
                    name,
                    field_type.data(),
                    [field](call_context& ctx) {
                        T* obj = static_cast<T*>(ctx.get_this());
                        ctx.store_result(obj->*field);
                    },
                    [field](call_context& ctx) {
                        T* obj = static_cast<T*>(ctx.get_this());
                        obj->*field = ctx.get_parameter<F>(0);
                    });
            }
            return *this;
        }

        template <typename R, typename... Args>
            requires !std::is_void_v<R>
                     type<T> &
            method(const char* name, R (T::*method)(Args...))
        {
            type<R> return_type;

            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {make_type_identifier<Args>()...};
            m_data->class_specific().methods.emplace_back(
                name,
                return_type.data(),
                sig,
                [method](call_context& ctx) {
                    try {
                        T*     obj = static_cast<T*>(ctx.get_this());
                        size_t index{0};
                        ctx.store_result((obj->*method)(
                            ctx.get_parameter<Args>(index++)...));
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                });
            return *this;
        }

        template <typename R, typename... Args>
            requires !std::is_void_v<R>
                     type<T> &
            method(const char* name, R (T::*method)(Args...) const)
        {
            type<R>                    return_type;
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {typeid(Args)...};
            m_data->class_specific().methods.emplace_back(
                name,
                return_type.data(),
                sig,
                [method](call_context& ctx) {
                    try {
                        T*     obj = static_cast<T*>(ctx.get_this());
                        size_t index{0};
                        ctx.store_result((obj->*method)(
                            ctx.get_parameter<Args>(index++)...));
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                });
            return *this;
        }

        template <typename... Args>
        type<T>& method(const char* name, void (T::*method)(Args...))
        {
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {typeid(Args)...};
            m_data.class_specific().methods.emplace_back(
                name,
                type<void>().data(),
                sig,
                [method](call_context& ctx) {
                    try {
                        T*     obj = static_cast<T*>(ctx.get_this());
                        size_t index{0};
                        (obj->*method)(ctx.get_parameter<Args>(index++)...);
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                });
            return *this;
        }

        template <typename... Args>
        type<T>& function(const char* name, void (*func)(Args...))
        {
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {make_type_identifier<Args>()...};
            m_data->class_specific().functions.emplace_back(
                name,
                type<void>().data(),
                sig,
                [func](call_context& ctx) {
                    try {
                        size_t index{0};
                        func(ctx.get_parameter<Args>(index++)...);
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                });
            return *this;
        }

        template <typename R, typename... Args>
            requires !std::is_void_v<R>
                     type<T> &
            function(const char* name, R (*func)(Args...))
        {
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {typeid(Args)...};
            m_data->class_specific().functions.emplace_back(
                name,
                type<R>().data(),
                sig,
                [func](call_context& ctx) {
                    try {
                        size_t index{0};
                        ctx.store_result(
                            func(ctx.get_parameter<Args>(index++)...));
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                    } catch (...) {
                        ctx.exception_thrown();
                    }
                });
            return *this;
        }

        template <typename Proxy>
            requires std::is_base_of_v<proxy<T>, Proxy>
        type<T>& proxy()
        {
            type<Proxy> proxy_type;
            m_data->class_specific().proxy_type = proxy_type.data();
            proxy_type.data()->class_specific().interface_type = m_data;
            return *this;
        }

    private:
        void initialize()
        {
            if constexpr (std::is_same_v<T, std::string>) {
                m_data->class_specific().is_string = true;
            } else if constexpr (std::is_same_v<T, std::wstring>) {
                m_data->class_specific().is_wstring = true;
            }
            m_data->class_specific().size = sizeof(T);
            if constexpr (std::is_abstract_v<T>) {
                m_data->class_specific().is_abstract = true;
            }
            if constexpr (!std::is_same_v<T, std::string> &&
                          !std::is_same_v<T, std::wstring> &&
                          !std::is_same_v<T, std::string_view> &&
                          !std::is_same_v<T, std::wstring_view>) {
                if constexpr (std::is_destructible_v<T>) {
                    m_data->class_specific().destroy = [](call_context& ctx) {
                        T* obj = static_cast<T*>(ctx.get_this());
                        obj->~T();
                    };
                }
                if constexpr (std::is_default_constructible_v<T>) {
                    m_data->class_specific().default_construct =
                        [](call_context& ctx) {
                            T* obj = static_cast<T*>(ctx.get_this());
                            new (obj) T();
                        };
                }
                if constexpr (std::is_copy_constructible_v<T>) {
                    type_data::call_signature sig = {
                        make_type_identifier<const T&>()};
                    m_data->class_specific().constructors.emplace_back(
                        sig,
                        [](call_context& ctx) {
                            T* obj = static_cast<T*>(ctx.get_this());
                            new (obj) T(ctx.get_parameter<const T&>(0));
                        });
                }
            }
        }

        type_data_ref m_data;
    };

    template <typename R, typename... Args>
    class type<std::function<R(Args...)>>
    {
    public:
        type()
        {
            type_identifier id =
                make_type_identifier<std::function<R(Args...)>>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data =
                    type_data::create(typeid(std::function<R(Args...)>), id);
                m_data->class_specific().is_callable = true;
            }
        }

        bool is_void() const noexcept { return false; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return true; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool directly_exposed() const noexcept
        {
            return m_data->directly_exposed();
        }

    private:
        type_data_ref m_data;
    };

    template <typename T> class type<T*>
    {
    public:
        type()
        {
            type_identifier id = make_type_identifier<T*>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(T*), id);
                // plain pointee type, no const here
                type<std::remove_cv_t<T>> pointee_type;
                m_data->pointer_specific().pointee = pointee_type.data();
            }
        }

        bool is_void() const noexcept { return false; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return false; }
        bool is_pointer() const noexcept { return true; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool directly_exposed() const noexcept
        {
            return m_data->directly_exposed();
        }

    private:
        type_data_ref m_data;
    };

    template <typename T>
        requires(std::is_reference_v<T> && !std::is_rvalue_reference_v<T>)
    class type<T>
    {
    public:
        type()
        {
            type_identifier id = make_type_identifier<T>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(T), id);
                type<std::remove_cv_t<std::remove_reference_t<T>>>
                    referencee_type;
                m_data->reference_specific().referencee =
                    referencee_type.data();
            }
        }

        bool is_void() const noexcept { return false; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return false; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return false; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool directly_exposed() const noexcept
        {
            return m_data->directly_exposed();
        }

    private:
        type_data_ref m_data;
    };

    template <typename T>
        requires std::is_rvalue_reference_v<T>
    class type<T>
    {
    public:
        type()
        {
            type_identifier id = make_type_identifier<T>();
            m_data = type_data::get(id);
            if (!m_data) {
                m_data = type_data::create(typeid(T), id);
                type<std::remove_cv_t<std::remove_reference_t<T>>>
                    referencee_type;
                m_data->rvalue_reference_specific().referencee =
                    referencee_type.data();
            }
        }

        bool is_void() const noexcept { return false; }
        bool is_bool() const noexcept { return false; }
        bool is_enum() const noexcept { return false; }
        bool is_pod() const noexcept { return false; }
        bool is_class() const noexcept { return false; }
        bool is_pointer() const noexcept { return false; }
        bool is_reference() const noexcept { return true; }
        bool is_rvalue_reference() const noexcept { return true; }

        const type_data_ref& data() const noexcept { return m_data; }

        bool directly_exposed() const noexcept
        {
            return m_data->directly_exposed();
        }

    private:
        type_data_ref m_data;
    };

} // namespace mge::script