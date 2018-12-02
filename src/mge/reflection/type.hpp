// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/module.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/reflection/signature.hpp"
#include "mge/reflection/type_definition.hpp"
#include "mge/reflection/parameter_source.hpp"
#include "mge/reflection/detail.hpp"
#include "mge/reflection/field.hpp"
#include "mge/reflection/method.hpp"
#include <string>
#include <map>
#include <typeindex>
#include <type_traits>
#include <boost/type_traits.hpp>

#include <iostream>

namespace mge {
    namespace reflection {


        template <typename TypeOfT, typename T, bool NOCLASS>
        struct class_type_base
        {
            typedef T reflected_type;
            typedef TypeOfT self_type;
        };

        template<typename TypeOfT, typename T>
        struct class_type_base<TypeOfT, T, false>
        {
            typedef T reflected_type;
            typedef TypeOfT self_type;

            template <typename ReturnType>
            self_type& method(const char *name,
                              ReturnType (reflected_type::* mptr)())
            {
                signature s;
                function_signature fs(type<ReturnType>().index(),
                                      s);
                auto f = [mptr](return_value& rv,
                                void* thisptr,
                                const parameter_source& src) -> void {
                    auto self = static_cast<reflected_type*>(thisptr);
                    set_return_value(rv, ((self)->*(mptr))());
                };
                base().definition()->method(name, fs, f, false);
                return base();
            }

            template <>
            self_type& method(const char *name,
                              void (reflected_type::* mptr)())
            {
                signature s;
                function_signature fs(type<void>().index(),
                                      s);
                auto f = [mptr](return_value& rv,
                                void* thisptr,
                                const parameter_source& src) -> void {
                    auto self = static_cast<reflected_type*>(thisptr);
                    ((self)->*(mptr))();
                };
                base().definition()->method(name, fs, f, false);
                return base();
            }

            template <typename ReturnType>
            self_type& method(const char *name,
                              ReturnType (reflected_type::* mptr)() const)
            {
                signature s;
                function_signature fs(type<ReturnType>().index(),
                                      s);
                auto f = [mptr](return_value& rv,
                                void* thisptr,
                                const parameter_source& src) -> void {
                    auto self = static_cast<const reflected_type*>(thisptr);
                    set_return_value(rv, ((self)->*(mptr))());
                };
                base().definition()->method(name, fs, f, false);
                return base();
            }

            template <>
            self_type& method(const char *name,
                              void (reflected_type::* mptr)() const)
            {
                signature s;
                function_signature fs(type<void>().index(),
                                      s);
                auto f = [mptr](return_value& rv,
                                void* thisptr,
                                const parameter_source& src) -> void {
                    auto self = static_cast<const reflected_type*>(thisptr);
                    ((self)->*(mptr))();
                };
                base().definition()->method(name, fs, f, false);
                return base();
            }

            template <typename ReturnType,
                      typename... Args>
            self_type& method(const char *name,
                              ReturnType (reflected_type::* mptr)(Args...))
            {
                signature s({type<Args>().index()...});
                function_signature fs(type<ReturnType>().index(),
                                      s);
                auto f = [mptr](return_value& rv,
                                void* thisptr,
                                const parameter_source& src) -> void {
                    auto self = static_cast<reflected_type*>(thisptr);
                    set_return_value(rv, ((self)->*(mptr))(parameter<Args>(src)...));
                };
                base().definition()->method(name, fs, f, false);
                return base();
            }

            template <typename ReturnType,
                      typename... Args>
            self_type& method(const char *name,
                              ReturnType (reflected_type::* mptr)(Args...) const)
            {
                signature s({type<Args>().index()...});
                function_signature fs(type<ReturnType>().index(),
                                      s);
                auto f = [mptr](return_value& rv,
                                void* thisptr,
                                const parameter_source& src) -> void {
                    auto self = static_cast<const reflected_type*>(thisptr);
                    set_return_value(rv, ((self)->*(mptr))(parameter<Args>(src)...));
                };
                base().definition()->method(name, fs, f, true);
                return base();
            }


            self_type& base()
            {
                return static_cast<self_type&>(*this);
            }
        };

        template <typename TypeOfT, typename T, bool NOENUM>
        struct enum_type_base
        {
            typedef T reflected_type;
            typedef TypeOfT self_type;
        };



        template <typename TypeOfT, typename T>
        struct enum_type_base<TypeOfT, T, true>
        {
            typedef T reflected_type;
            typedef TypeOfT self_type;

            self_type& base()
            {
                return static_cast<self_type&>(*this);
            }

            self_type& enum_value(const char *name, T value)
            {
  //              base().definition()->(name, (int64_t)value);
                return base();
            }

        };

        template <typename T>
        class type
            : public class_type_base<type<T>, T, !boost::is_class<T>::value>
            , public enum_type_base<type<T>, T, boost::is_enum<T>::value>
        {
        private:
            typedef class_type_base<type<T>, T, !boost::is_class<T>::value> method_base_type;
        public:
            typedef type<T> self_type;
            typedef T reflected_type;

            type()
            {
                module& m = module::get<T>();
                m_definition = m.type<T>();
                if(!m_definition) {
                    m_definition = std::make_shared<type_definition>
                        (base_type_name<T>(),
                         std::type_index(typeid(T)),
                         detail::size_of_type<T>(),
                         boost::is_enum<T>::value);
                    m.type(m_definition);
                }
            }
            const std::string name() const { return m_definition->name(); }
            size_t size() const { return m_definition->size(); }


            template <typename F, typename U=T>
            self_type& field(const char *name,
                             F typename U::* f)
            {
                auto t = mge::reflection::type<U>();
                definition()->field(name, t.definition());
                return *this;
            }




            self_type& constructor()
            {
                auto cf = [](void *ptr, const parameter_source&) -> void {
                    new (ptr) T();
                };
                m_definition->constructor(signature(), cf);
                return *this;
            }

            template <typename... Args>
            self_type& constructor()
            {
                signature s({std::type_index(typeid(Args))...});
                auto cf = [](void *ptr, const parameter_source& src) -> void {
                    new (ptr) T(parameter<Args>(src)...);
                };
                m_definition->constructor(signature(), cf);
                return *this;
            }


            self_type& destructor()
            {
                auto cf = [](void *ptr) {
                    reinterpret_cast<T*>(ptr)->~T();
                };
                m_definition->destructor(cf);
                return *this;
            }

            const type_definition_ref& definition() const { return m_definition; }
            std::type_index index() const { return definition()->index(); }
        private:
            type_definition_ref m_definition;
        };

        template <>
        class type<void>
        {
        public:
            typedef type<void> self_type;
            typedef void reflected_type;

            type()
            {
                module& m = module::get<void>();
                m_definition = m.type<void>();
                if(!m_definition) {
                    m_definition = std::make_shared<type_definition>
                        (base_type_name<void>(),
                         std::type_index(typeid(void)),
                         detail::size_of_type<void>(),
                         false);
                    m.type(m_definition);
                }
            }

            const std::string name() const { return m_definition->name(); }
            size_t size() const { return m_definition->size(); }
            const type_definition_ref& definition() const { return m_definition; }
            std::type_index index() const { return definition()->index(); }
        private:
            type_definition_ref m_definition;
        };

    }
}

#define explicit_method(R, C, N, ARGS) method(#N, static_cast<R (C::*)ARGS>(&C::N))
