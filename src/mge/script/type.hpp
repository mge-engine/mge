// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/call_debugger.hpp"
#include "mge/core/callable.hpp"
#include "mge/core/nth_type.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/call_context.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/parameter_retriever.hpp"
#include "mge/script/proxy.hpp"
#include "mge/script/result_storer.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/signature.hpp"
#include "mge/script/traits.hpp"

#include <functional>
#include <iostream>
#include <string>

namespace mge {
#ifdef BUILD_SCRIPT
    MGE_USE_TRACE(SCRIPT);
#else
    MGE_USE_IMPORTED_TRACE(SCRIPT);
#endif
} // namespace mge

namespace mge::script {

    /**
     * @brief Base type for type representations.
     */
    class MGESCRIPT_EXPORT type_base
    {
    public:
        /**
         * @brief Default constructor.
         */
        type_base();

        /**
         * @brief Assigment
         * @param t assigned type
         * @return @c *this
         */
        type_base& operator=(const type_base& t);

        /**
         * @brief Destructor.
         */
        ~type_base() = default;

        const std::string&      name() const;
        mge::script::module     module() const;
        const std::type_index&  type_index() const;
        const type_details_ref& details() const;
        type_details_ref&       details();

    protected:
        void init_details(const std::type_index& index);
        void init_enum_details(const std::type_index& index,
                               const std::string&     name,
                               const traits&          tr,
                               size_t                 size);
        void init_class_details(const std::type_index& index,
                                const std::string&     name,
                                const traits&          tr,
                                size_t                 size);
        void get_class_details(const std::type_index& index);
        void set_base(const type_details_ref& base_details);
        void set_destructor(const invoke_function& delete_ptr,
                            const invoke_function& delete_shared_ptr);
        void add_constructor(const signature&       signature,
                             const invoke_function& ctor,
                             const invoke_function& make_shared);

        void add_method(const std::string&     name,
                        const std::type_index& return_type,
                        const signature&       signature,
                        const invoke_function& invoke);

        void add_static_method(const std::string&     name,
                               const std::type_index& return_type,
                               const signature&       signature,
                               const invoke_function& invoke);

        void add_field(const std::string&     name,
                       const type_base&       type,
                       const invoke_function& getter);
        void add_field(const std::string&     name,
                       const type_base&       type,
                       const invoke_function& getter,
                       const invoke_function& setter);

        void enum_value(const std::string& name, int64_t value);

        void add_member(type_base& t);

    private:
        type_details_ref m_details;
    };

    /**
     * @brief Type class.
     *
     * @tparam T type represented by class
     * @tparam typename type used for SFINAE, must resolve to @c void
     */
    template <typename T, typename = void> class type : public type_base
    {
    public:
        inline type();
    };

    /**
     * @brief Simple type specialization.
     *
     * @tparam T type represented, must be simple type
     */
    template <typename T>
    class type<T, typename std::enable_if<is_simple_type<T>>::type>
        : public type_base
    {
    public:
        inline type() { init_details(std::type_index(typeid(T))); }

        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;
    };

    template <typename T>
    class type<T, typename std::enable_if<std::is_enum_v<T>>::type>
        : public type_base
    {
    public:
        inline explicit type(const std::string& n)
        {
            auto ti = std::type_index(typeid(T));
            auto tr = traits_of<T>();
            init_enum_details(ti, n, tr, sizeof(T));
        }

        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::base_type_name<T>();
            auto tr = traits_of<T>();
            init_enum_details(ti, n, tr, sizeof(T));
        }

        auto& enum_value(const std::string& name, T value)
        {
            type_base::enum_value(name, static_cast<int64_t>(value));
            return *this;
        }

        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;
    };

    template <typename R, typename... Args>
    class type<std::function<R(Args...)>, void> : public type_base
    {
    private:
        using self_type = std::function<R(Args...)>;

    public:
        inline explicit type()
        {
            auto ti = std::type_index(typeid(self_type));
            init_details(ti);
        }

        inline explicit type(const std::string& n)
        {
            auto ti = std::type_index(typeid(self_type));
            auto tr = traits_of<self_type>();
            init_class_details(ti, n, tr, sizeof(self_type));
            add_default_constructor();

            if constexpr (std::is_destructible_v<self_type>) {
                if constexpr (std::is_trivially_destructible_v<self_type>) {
                    set_destructor(
                        [](call_context& context) {},
                        [](call_context& context) {
                            void* shared_ptr_addr_untyped =
                                context.shared_ptr_address();
                            std::shared_ptr<self_type>** shared_ptr_addr =
                                reinterpret_cast<std::shared_ptr<self_type>**>(
                                    shared_ptr_addr_untyped);
                            delete (*shared_ptr_addr);
                            (*shared_ptr_addr) = nullptr;
                        });
                } else {
                    set_destructor(
                        [](call_context& context) {
                            self_type* self = reinterpret_cast<self_type*>(
                                context.this_ptr());
                            self->~self_type();
                        },
                        [](call_context& context) {
                            void* shared_ptr_addr_untyped =
                                context.shared_ptr_address();
                            std::shared_ptr<self_type>** shared_ptr_addr =
                                reinterpret_cast<std::shared_ptr<self_type>**>(
                                    shared_ptr_addr_untyped);
                            delete (*shared_ptr_addr);
                            (*shared_ptr_addr) = nullptr;
                        });
                }
            }
        }

    private:
        void add_default_constructor()
        {
            auto new_at = [](call_context& ctx) {
                new (ctx.this_ptr()) self_type();
            };
            auto new_shared = [](call_context& context) {
                void* shared_ptr_addr_untyped = context.shared_ptr_address();
                std::shared_ptr<self_type>** shared_ptr_addr =
                    reinterpret_cast<std::shared_ptr<self_type>**>(
                        shared_ptr_addr_untyped);
                (*shared_ptr_addr) = new std::shared_ptr<self_type>();
                (**shared_ptr_addr) = std::make_shared<self_type>();
            };
            signature empty;
            add_constructor(empty, new_at, new_shared);
        }
    };

    template <typename T>
    class type<T,
               typename std::enable_if<std::is_class_v<T> &&
                                       !mge::is_callable_v<T>>::type>
        : public type_base
    {
    private:
        template <typename... ConstructorArgs> struct constructor_helper
        {
            template <std::size_t... I>
            static void construct(call_context& context,
                                  std::index_sequence<I...>)
            {
                if constexpr ((sizeof...(I) == 1) &&
                              std::is_same_v<nth_type<0, ConstructorArgs...>,
                                             void>) {
                    new (context.this_ptr()) T();
                } else {
                    new (context.this_ptr())
                        T(parameter_retriever<nth_type<I, ConstructorArgs...>>(
                              context,
                              I)
                              .get()...);
                }
            }
        };

        template <typename... ConstructorArgs> struct new_shared_helper
        {
            template <std::size_t... I>
            static void new_shared(call_context& context,
                                   std::index_sequence<I...>)
            {
                void* shared_ptr_address_untyped = context.shared_ptr_address();
                std::shared_ptr<T>** shared_ptr_address =
                    reinterpret_cast<std::shared_ptr<T>**>(
                        shared_ptr_address_untyped);
                (*shared_ptr_address) = new std::shared_ptr<T>();
                if constexpr ((sizeof...(I) == 1) &&
                              std::is_same_v<nth_type<0, ConstructorArgs...>,
                                             void>) {
                    (**shared_ptr_address) = std::make_shared<T>();
                } else {
                    (**shared_ptr_address) = std::make_shared<T>(
                        parameter_retriever<nth_type<I, ConstructorArgs...>>(
                            context,
                            I)
                            .get()...);
                }
            }
        };

    public:
        using self_type =
            type<T, typename std::enable_if<std::is_class_v<T>>::type>;

        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            init_details(ti);
        }

        inline explicit type(const std::string& n)
        {
            auto ti = std::type_index(typeid(T));
            auto tr = traits_of<T>();
            init_class_details(ti, n, tr, sizeof(T));
            if constexpr (std::is_destructible_v<T>) {
                if constexpr (std::is_trivially_destructible_v<T>) {
                    set_destructor(
                        [](call_context& context) {},
                        [](call_context& context) {
                            void* shared_ptr_addr_untyped =
                                context.shared_ptr_address();
                            std::shared_ptr<T>** shared_ptr_addr =
                                reinterpret_cast<std::shared_ptr<T>**>(
                                    shared_ptr_addr_untyped);
                            delete (*shared_ptr_addr);
                            (*shared_ptr_addr) = nullptr;
                        });
                } else {
                    set_destructor(
                        [](call_context& context) {
                            T* self = reinterpret_cast<T*>(context.this_ptr());
                            self->~T();
                        },
                        [](call_context& context) {
                            void* shared_ptr_addr_untyped =
                                context.shared_ptr_address();
                            std::shared_ptr<T>** shared_ptr_addr =
                                reinterpret_cast<std::shared_ptr<T>**>(
                                    shared_ptr_addr_untyped);
                            delete (*shared_ptr_addr);
                            (*shared_ptr_addr) = nullptr;
                        });
                }
            }
        }

        template <typename... ConstructorArgs> self_type& constructor()
        {
            std::array<std::type_index, sizeof...(ConstructorArgs)> arg_types =
                {std::type_index(typeid(ConstructorArgs))...};
            auto construct = [](call_context& ctx) {
                constructor_helper<ConstructorArgs...>::construct(
                    ctx,
                    std::make_index_sequence<sizeof...(ConstructorArgs)>{});
            };
            auto new_shared = [](call_context& ctx) {
                new_shared_helper<ConstructorArgs...>::new_shared(
                    ctx,
                    std::make_index_sequence<sizeof...(ConstructorArgs)>{});
            };
            signature s(arg_types);
            add_constructor(s, construct, new_shared);
            return *this;
        }

        inline self_type& constructor()
        {
            auto new_at = [](call_context& ctx) { new (ctx.this_ptr()) T(); };
            auto new_shared = [](call_context& context) {
                void* shared_ptr_addr_untyped = context.shared_ptr_address();
                std::shared_ptr<T>** shared_ptr_addr =
                    reinterpret_cast<std::shared_ptr<T>**>(
                        shared_ptr_addr_untyped);
                (*shared_ptr_addr) = new std::shared_ptr<T>();
                (**shared_ptr_addr) = std::make_shared<T>();
            };
            signature empty;
            add_constructor(empty, new_at, new_shared);
            return *this;
        }

        inline self_type copy_constructor() { return constructor<const T&>(); }

        inline self_type move_constructor() { return constructor<T&&>(); }

        template <typename TB, typename TV>
        std::enable_if<std::is_base_of_v<TB, T>, self_type>::type&
        base(const type<TB, TV>& base_type)
        {
            set_base(base_type.details());
            return *this;
        }

        template <typename F>
        self_type& field(const std::string& name, F T::*fieldptr)
        {
            type<F> field_type;
            if constexpr (std::is_const_v<F>) {
                auto getter = [fieldptr](call_context& ctx) {
                    const T* objptr = static_cast<const T*>(ctx.this_ptr());
                    result_storer<F>::store(ctx, objptr->*fieldptr);
                };
                add_field(name, field_type, getter);
            } else {
                auto getter = [fieldptr](call_context& ctx) {
                    T* objptr = static_cast<T*>(ctx.this_ptr());
                    result_storer<F>::store(ctx, objptr->*fieldptr);
                };
                auto setter = [fieldptr](call_context& ctx) {
                    T* objptr = static_cast<T*>(ctx.this_ptr());
                    objptr->*fieldptr = parameter_retriever<F>(ctx, 0).get();
                };
                add_field(name, field_type, getter, setter);
            }
            return *this;
        }

    private:
        template <typename... MethodArgs> struct method_helper
        {
            template <typename R, std::size_t... I>
            static inline void call(call_context& ctx,
                                    R (T::*mptr)(MethodArgs...),
                                    std::index_sequence<I...>)
            {
                T* objptr = static_cast<T*>(ctx.this_ptr());
                if constexpr (std::is_void_v<R>) {
                    (objptr->*mptr)(
                        parameter_retriever<nth_type<I, MethodArgs...>>(ctx, I)
                            .get()...);
                } else {
                    result_storer<R>::store(
                        ctx,
                        (objptr->*mptr)(
                            parameter_retriever<nth_type<I, MethodArgs...>>(ctx,
                                                                            I)
                                .get()...));
                }
            }

            template <typename R, std::size_t... I>
            static inline void call(call_context& ctx,
                                    R (T::*mptr)(MethodArgs...) const,
                                    std::index_sequence<I...>)
            {
                T* objptr = static_cast<T*>(ctx.this_ptr());
                if constexpr (std::is_void_v<R>) {
                    (objptr->*mptr)(
                        parameter_retriever<nth_type<I, MethodArgs...>>::get(
                            ctx,
                            I)...);
                } else {
                    result_storer<R>::store(
                        ctx,
                        (objptr->*mptr)(
                            parameter_retriever<nth_type<I, MethodArgs...>>(ctx,
                                                                            I)
                                .get()...));
                }
            }

            template <typename InvokeResult, std::size_t... I>
            static inline void
            call_cfunction(InvokeResult (*fptr)(MethodArgs...),
                           call_context& context,
                           std::index_sequence<I...>)
            {
                if constexpr (std::is_void_v<InvokeResult>) {
                    (*fptr)(
                        parameter_retriever<nth_type<I, MethodArgs...>>::get(
                            context,
                            I)...);
                } else {
                    result_storer<InvokeResult>::store(
                        context,
                        (*fptr)(parameter_retriever<nth_type<I, MethodArgs...>>(
                                    context,
                                    I)
                                    .get()...));
                }
            }
        };

    public:
        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name, R (T::*mptr)(MethodArgs...))
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name,
                          R (T::*mptr)(MethodArgs...) noexcept)
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name,
                          R (T::*mptr)(MethodArgs...) const)
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name,
                          R (T::*mptr)(MethodArgs...) const noexcept)
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [mptr](call_context& ctx) {
                method_helper<MethodArgs...>::call(
                    ctx,
                    mptr,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));

            add_method(name, result_type, s, call);
            return *this;
        }

        template <typename R, typename... MethodArgs>
        self_type& method(const std::string& name, R (*fptr)(MethodArgs...))
        {
            std::array<std::type_index, sizeof...(MethodArgs)> arg_types = {
                std::type_index(typeid(MethodArgs))...};
            auto call = [fptr](call_context& ctx) {
                method_helper<MethodArgs...>::call_cfunction(
                    fptr,
                    ctx,
                    std::make_index_sequence<sizeof...(MethodArgs)>{});
            };
            signature s(arg_types);
            auto      result_type = std::type_index(typeid(R));
            add_static_method(name, result_type, s, call);
            return *this;
        }

        /**
         * @brief Add elements to the type
         *
         * @tparam SubType        type of first element
         * @tparam SubArgs     other element types
         * @param arg0      first element
         * @param args      other elements
         *
         * @return @c *this
         */
        template <typename SubType, typename... SubArgs>
        self_type& operator()(const SubType& arg0, const SubArgs&... args)
        {
            add_member(arg0);
            return operator()(args...);
        }

        self_type& operator()() { return *this; }

        template <typename SubType, typename SubTypeDetail>
        void add_member(const type<SubType, typename SubTypeDetail>& t)
        {
            type_base::add_member(
                const_cast<type_base&>(static_cast<const type_base&>(t)));
        }
    };

} // namespace mge::script