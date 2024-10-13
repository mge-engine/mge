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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
        }

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
                m_data->enum_specific().name = mge::enum_type_name<T>();
                m_data->enum_specific().underlying_type =
                    type_data::get(underlying_id);
                m_data->add_dependency(
                    dependency(m_data->enum_specific().underlying_type));
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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
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
                m_data = type_data::create(typeid(T), id);
                initialize();
                m_data->set_alias_name(alias_name);
            } else {
                if (m_data->alias_name() != alias_name) {
                    MGE_THROW(illegal_state)
                        << "Type " << m_data->name()
                        << " already exists with different alias name";
                }
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

        template <typename B>
        type<T>& base()
            requires std::is_base_of_v<B, T>
        {
            type<B> base_type;
            m_data->class_specific().base_classes.emplace_back(
                base_type.data());
            m_data->add_dependency(dependency(base_type.data()));
            return *this;
        }

        template <typename... Args> type<T>& constructor()
        {
            if constexpr (std::is_abstract_v<T>) {
                MGE_THROW(illegal_state)
                    << "Cannot create constructor for abstract class";
            }
            type_data::call_signature sig = {make_type_identifier<Args>()...};
            (m_data->add_dependency(dependency(type<Args>().data())), ...);
            m_data->class_specific().constructors.emplace_back(
                sig,
                [](call_context& ctx) {
                    T*               obj = static_cast<T*>(ctx.this_ptr());
                    constexpr size_t nargs = sizeof...(Args);
                    size_t           index{nargs};
                    ctx.before_call();
                    new (obj) T((ctx.parameter<Args>(--index))...);
                    ctx.after_call();
                });
            m_data->class_specific().make_shared_constructors.emplace_back(
                sig,
                [](call_context& ctx) {
                    std::shared_ptr<T>** obj =
                        static_cast<std::shared_ptr<T>**>(
                            ctx.shared_ptr_address());
                    *obj = new std::shared_ptr<T>();
                    constexpr size_t nargs = sizeof...(Args);
                    size_t           index{nargs};
                    ctx.before_call();
                    **obj =
                        std::make_shared<T>((ctx.parameter<Args>(--index))...);
                    ctx.after_call();
                });
            return *this;
        }

        template <typename F> type<T>& field(const char* name, F T::*field)
        {
            type<F> field_type;
            m_data->add_dependency(dependency(field_type.data()));
            if constexpr (std::is_const_v<F>) {
                m_data->class_specific().fields.emplace_back(
                    name,
                    field_type.data(),
                    [field](call_context& ctx) {
                        T* obj = static_cast<T*>(ctx.this_ptr());
                        ctx.before_call();
                        ctx.result(obj->*field);
                        ctx.after_call();
                    },
                    nullptr);
            } else {
                m_data->class_specific().fields.emplace_back(
                    name,
                    field_type.data(),
                    [field](call_context& ctx) {
                        T* obj = static_cast<T*>(ctx.this_ptr());
                        ctx.before_call();
                        ctx.result(obj->*field);
                        ctx.after_call();
                    },
                    [field](call_context& ctx) {
                        T* obj = static_cast<T*>(ctx.this_ptr());
                        ctx.before_call();
                        obj->*field = ctx.parameter<F>(0);
                        ctx.after_call();
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
            m_data->add_dependency(dependency(return_type.data()));
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            (m_data->add_dependency(dependency(type<Args>().data())), ...);
            type_data::call_signature sig = {make_type_identifier<Args>()...};
            m_data->class_specific().methods.emplace_back(
                name,
                return_type.data(),
                sig,
                [method](call_context& ctx) {
                    try {
                        T*               obj = static_cast<T*>(ctx.this_ptr());
                        constexpr size_t nargs = sizeof...(Args);
                        size_t           index{nargs};
                        ctx.before_call();
                        ctx.result(
                            (obj->*method)((ctx.parameter<Args>(--index))...));
                        ctx.after_call();
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (...) {
                        ctx.exception_thrown();
                        ctx.after_call();
                    }
                });
            return *this;
        }

        template <typename R, typename... Args>
            requires !std::is_void_v<R>
                     type<T> &
            method(const char* name, R (T::*method)(Args...) const)
        {
            type<R> return_type;
            m_data->add_dependency(dependency(return_type.data()));
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            (m_data->add_dependency(dependency(type<Args>().data())), ...);
            type_data::call_signature sig = {make_type_identifier<Args>()...};
            m_data->class_specific().methods.emplace_back(
                name,
                return_type.data(),
                sig,
                [method](call_context& ctx) {
                    try {
                        T*               obj = static_cast<T*>(ctx.this_ptr());
                        constexpr size_t nargs = sizeof...(Args);
                        size_t           index{nargs};
                        ctx.before_call();
                        ctx.result(
                            (obj->*method)(ctx.parameter<Args>(--index)...));
                        ctx.after_call();
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (...) {
                        ctx.exception_thrown();
                        ctx.after_call();
                    }
                });
            return *this;
        }

        template <typename... Args>
        type<T>& method(const char* name, void (T::*method)(Args...))
        {
            m_data->add_dependency(dependency(type<void>().data()));
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {make_type_identifier<Args>()...};
            (m_data->add_dependency(dependency(type<Args>().data())), ...);
            m_data->class_specific().methods.emplace_back(
                name,
                type<void>().data(),
                sig,
                [method](call_context& ctx) {
                    try {
                        T*               obj = static_cast<T*>(ctx.this_ptr());
                        constexpr size_t nargs = sizeof...(Args);
                        size_t           index{nargs};
                        ctx.before_call();
                        (obj->*method)(ctx.parameter<Args>(--index)...);
                        ctx.after_call();
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (...) {
                        ctx.exception_thrown();
                        ctx.after_call();
                    }
                });
            return *this;
        }

        template <typename... Args>
        type<T>& function(const char* name, void (*func)(Args...))
        {
            m_data->add_dependency(dependency(type<void>().data()));
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {make_type_identifier<Args>()...};
            (m_data->add_dependency(dependency(type<Args>().data())), ...);
            m_data->class_specific().functions.emplace_back(
                name,
                type<void>().data(),
                sig,
                [func](call_context& ctx) {
                    try {
                        constexpr size_t nargs = sizeof...(Args);
                        size_t           index{nargs};
                        ctx.before_call();
                        func(ctx.parameter<Args>(--index)...);
                        ctx.after_call();
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (...) {
                        ctx.exception_thrown();
                        ctx.after_call();
                    }
                });
            return *this;
        }

        template <typename R, typename... Args>
            requires !std::is_void_v<R>
                     type<T> &
            function(const char* name, R (*func)(Args...))
        {
            m_data->add_dependency(dependency(type<R>().data()));
            std::vector<type_data_ref> arg_types = {type<Args>().data()...};
            type_data::call_signature  sig = {make_type_identifier<Args>()...};
            (m_data->add_dependency(dependency(type<Args>().data())), ...);
            m_data->class_specific().functions.emplace_back(
                name,
                type<R>().data(),
                sig,
                [func](call_context& ctx) {
                    try {
                        constexpr size_t nargs = sizeof...(Args);
                        size_t           index{nargs};
                        ctx.before_call();
                        ctx.result(func(ctx.parameter<Args>(--index)...));
                        ctx.after_call();
                    } catch (const mge::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (const std::exception& e) {
                        ctx.exception_thrown(e);
                        ctx.after_call();
                    } catch (...) {
                        ctx.exception_thrown();
                        ctx.after_call();
                    }
                });
            return *this;
        }

        template <typename Proxy>
            requires std::is_base_of_v<proxy<T>, Proxy>
        type<T>& proxy()
        {
            type<Proxy> proxy_type;
            proxy_type.data()->add_dependency(dependency(m_data));
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
                m_data->class_specific().name = mge::base_type_name<T>();
                m_data->class_specific().this_from_shared_ptr =
                    [](void* shared_ptr_address) -> void* {
                    std::shared_ptr<T>* sp =
                        static_cast<std::shared_ptr<T>*>(shared_ptr_address);
                    return sp->get();
                };

                if constexpr (std::is_destructible_v<T>) {
                    m_data->class_specific().destroy = [](call_context& ctx) {
                        ctx.before_call();
                        T* obj = static_cast<T*>(ctx.this_ptr());
                        obj->~T();
                        ctx.after_call();
                    };
                    m_data->class_specific().destroy_shared =
                        [](call_context& ctx) {
                            ctx.before_call();
                            std::shared_ptr<T>** sp =
                                static_cast<std::shared_ptr<T>**>(
                                    ctx.shared_ptr_address());
                            if (sp) {
                                if (*sp) {
                                    delete *sp;
                                    *sp = nullptr;
                                }
                            }
                            ctx.after_call();
                        };
                }
                if constexpr (std::is_default_constructible_v<T>) {
                    type_data::call_signature sig = {};
                    m_data->class_specific().constructors.emplace_back(
                        sig,
                        [](call_context& ctx) {
                            try {
                                ctx.before_call();
                                T* obj = static_cast<T*>(ctx.this_ptr());
                                new (obj) T();
                                ctx.after_call();
                            } catch (const mge::exception& e) {
                                ctx.exception_thrown(e);
                                ctx.after_call();
                            } catch (const std::exception& e) {
                                ctx.exception_thrown(e);
                                ctx.after_call();
                            } catch (...) {
                                ctx.exception_thrown();
                                ctx.after_call();
                            }
                        });
                    m_data->class_specific()
                        .make_shared_constructors.emplace_back(
                            sig,
                            [](call_context& ctx) {
                                try {
                                    ctx.before_call();
                                    std::shared_ptr<T>** obj =
                                        static_cast<std::shared_ptr<T>**>(
                                            ctx.shared_ptr_address());
                                    *obj = new std::shared_ptr<T>();
                                    **obj = std::make_shared<T>();
                                    ctx.after_call();
                                } catch (const mge::exception& e) {
                                    ctx.exception_thrown(e);
                                    ctx.after_call();
                                } catch (const std::exception& e) {
                                    ctx.exception_thrown(e);
                                    ctx.after_call();
                                } catch (...) {
                                    ctx.exception_thrown();
                                    ctx.after_call();
                                }
                            });
                }
                if constexpr (std::is_copy_constructible_v<T>) {
                    type_data::call_signature sig = {
                        make_type_identifier<const T&>()};
                    m_data->class_specific().constructors.emplace_back(
                        sig,
                        [](call_context& ctx) {
                            try {
                                ctx.before_call();
                                T* obj = static_cast<T*>(ctx.this_ptr());
                                new (obj) T(ctx.parameter<const T&>(0));
                                ctx.after_call();
                            } catch (const mge::exception& e) {
                                ctx.exception_thrown(e);
                                ctx.after_call();
                            } catch (const std::exception& e) {
                                ctx.exception_thrown(e);
                                ctx.after_call();
                            } catch (...) {
                                ctx.exception_thrown();
                                ctx.after_call();
                            }
                        });
                    m_data->class_specific()
                        .make_shared_constructors.emplace_back(
                            sig,
                            [](call_context& ctx) {
                                try {
                                    ctx.before_call();
                                    std::shared_ptr<T>** obj =
                                        static_cast<std::shared_ptr<T>**>(
                                            ctx.shared_ptr_address());
                                    *obj = new std::shared_ptr<T>();
                                    **obj = std::make_shared<T>(
                                        ctx.parameter<const T&>(0));
                                    ctx.after_call();
                                } catch (const mge::exception& e) {
                                    ctx.exception_thrown(e);
                                    ctx.after_call();
                                } catch (const std::exception& e) {
                                    ctx.exception_thrown(e);
                                    ctx.after_call();
                                } catch (...) {
                                    ctx.exception_thrown();
                                    ctx.after_call();
                                }
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
                m_data->add_dependency(dependency(type<R>().data()));
                (m_data->add_dependency(dependency(type<Args>().data())), ...);
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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
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
                m_data->add_dependency(dependency(pointee_type.data()));
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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
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
                m_data->add_dependency(dependency(referencee_type.data()));
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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
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
                m_data->add_dependency(dependency(referencee_type.data()));
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

        const type_identifier& identifier() const noexcept
        {
            return m_data->identifier();
        }

    private:
        type_data_ref m_data;
    };

} // namespace mge::script