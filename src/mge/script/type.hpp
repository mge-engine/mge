// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/sfinae.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_classification.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

#include <iostream>
#include <list>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <typeindex>

namespace mge::script {

    namespace details {
        class MGESCRIPT_EXPORT type_base
        {
        public:
            type_base() = default;
            virtual ~type_base() = default;
            std::type_index         type_index() const;
            const std::string&      name() const;
            mge::script::module     module() const;
            const type_details_ref& details() const;

            static type_details_ref get_details(std::type_index index);
            static type_details_ref get_details_required(std::type_index index);

        protected:
            type_details_ref
            create_details(std::type_index            index,
                           const std::string&         name,
                           const type_classification& type_class);

            type_details_ref m_details;
        };

    } // namespace details

    template <typename T, typename = void>
    class type : public details::type_base
    {
    public:
        inline type()
        {
            static_assert(false, "Default expansion of type detected");
        }
    };

    template <> class type<void, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(void));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_void = 1};
                m_details = create_details(ti, "void", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::get_details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<char, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(char));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_char = 1};
                m_details = create_details(ti, "char", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<unsigned char, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(unsigned char));
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_char = 1};
                m_details = create_details(ti, "unsigned char", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<signed char, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(signed char));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_char = 1};
                m_details = create_details(ti, "signed char", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<short, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(short));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_short = 1};
                m_details = create_details(ti, "short", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<unsigned short, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(unsigned short));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_short = 1};
                m_details = create_details(ti, "unsigned short", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<int, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(int));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_int = 1};
                m_details = create_details(ti, "int", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<unsigned int, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(unsigned int));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_int = 1};
                m_details = create_details(ti, "unsigned int", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<int64_t, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(int64_t));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_long = 1};
                m_details = create_details(ti, "long", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<uint64_t, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(uint64_t));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_unsigned = 1, .is_long = 1};
                m_details = create_details(ti, "unsigned long", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<float, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(float));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_float = 1};
                m_details = create_details(ti, "float", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    template <> class type<double, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(double));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_double = 1};
                m_details = create_details(ti, "double", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
        using details::type_base::type_index;
    };

    // std types
    template <> class type<std::string, void> : public details::type_base
    {
    public:
        inline type()
        {
            auto ti = std::type_index(typeid(std::string));
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{};
                m_details = create_details(ti, "string", cls);
            }
        }

        using details::type_base::details;
        using details::type_base::name;
    };

    // enum type
    template <typename T>
    class type<T, typename std::enable_if<std::is_enum<T>::value>::type>
        : public details::type_base
    {
    public:
        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::base_type_name<T>();
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_enum = 1};
                m_details = create_details(ti, n, cls);
                m_enum_details =
                    std::dynamic_pointer_cast<enum_type_details>(m_details);
            }
        }

        template <typename I> auto& enum_value(const std::string& name, I value)
        {
            m_enum_details->add_value(name, value);
            return *this;
        }

        using details::type_base::details;
        using details::type_base::name;

    private:
        enum_type_details_ref m_enum_details;
    };

    // class type
    template <typename T>
    class type<T, typename std::enable_if<std::is_class<T>::value>::type>
        : public details::type_base
    {
    public:
        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::type_name<T>();
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_class = 1};
                m_details = create_details(ti, n, cls);
                m_class_details =
                    std::dynamic_pointer_cast<class_type_details>(m_details);
            }
        }

        inline explicit type(const std::string& alias_name)
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::type_name<T>();
            m_details = get_details(ti);
            if (!m_details) {
                type_classification cls{.is_class = 1};
                m_details = create_details(ti, n, cls);
                m_details->set_alias_name(alias_name);
                m_class_details =
                    std::dynamic_pointer_cast<class_type_details>(m_details);
            }
        }

        template <typename T, typename F>
        auto& field(const char* name, F T::*member_ptr)
        {
            return *this;
        }

        using details::type_base::details;
        using details::type_base::name;

    private:
        class_type_details_ref m_class_details;
    };

} // namespace mge::script