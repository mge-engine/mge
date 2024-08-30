// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/enum.hpp"
#include "mge/script/dllexport.hpp"

#include <iostream>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

namespace mge::script {

    /**
     * Identifier - or key - of a type. This identifier is used to identify
     * types and store information dependent on the exact type. Note that while
     * ``std::type_info`` is used to identify the type, that does not capture
     * all type properties, i.e. constness, volatility, and whether it is a
     * reference or rvalue reference.
     */
    class type_identifier
    {
    public:
        /** Classification. */
        enum kind_type : uint8_t
        {
            TYPE_VOID,    //!< Void type.
            TYPE_POD,     //!< Plain old data type, integer, float and boolean.
            TYPE_ENUM,    //!< Enumeration type.
            TYPE_CLASS,   //!< Class and struct type.
            TYPE_POINTER, //!< Pointer type.
            TYPE_REFERENCE,        //!< Reference type.
            TYPE_RVALUE_REFERENCE, //!< Rvalue reference type.
        };

        /**
         * @brief Construct a new type identifier object.
         * @param ti type information
         * @param kind type kind
         * @param is_const true if type is const
         * @param is_volatile true if type is volatile
         */
        type_identifier(const std::type_info& ti,
                        kind_type             kind,
                        bool                  is_const,
                        bool                  is_volatile)
            : m_type_index(ti)
            , m_kind(kind)
        {
            m_cv.is_const = is_const ? 1 : 0;
            m_cv.is_volatile = is_volatile ? 1 : 0;
        }

        type_identifier(const type_identifier&) = default;
        type_identifier& operator=(const type_identifier&) = default;

        type_identifier(type_identifier&&) = default;
        type_identifier& operator=(type_identifier&&) = default;

        ~type_identifier() = default;

        bool operator==(const type_identifier& other) const
        {
            return m_type_index == other.m_type_index &&
                   m_kind == other.m_kind &&
                   m_cv.is_const == other.m_cv.is_const &&
                   m_cv.is_volatile == other.m_cv.is_volatile;
        }

        bool operator!=(const type_identifier& other) const
        {
            return !(*this == other);
        }

        std::type_index type_index() const { return m_type_index; }
        kind_type       kind() const { return m_kind; }
        bool            is_const() const { return m_cv.is_const; }
        bool            is_volatile() const { return m_cv.is_volatile; }

        bool operator<(const type_identifier& other) const
        {
            return std::tie(m_type_index,
                            m_kind,
                            m_cv.is_const,
                            m_cv.is_volatile) <
                   std::tie(other.m_type_index,
                            other.m_kind,
                            other.m_cv.is_const,
                            other.m_cv.is_volatile);
        }

        bool operator>(const type_identifier& other) const
        {
            return std::tie(m_type_index,
                            m_kind,
                            m_cv.is_const,
                            m_cv.is_volatile) >
                   std::tie(other.m_type_index,
                            other.m_kind,
                            other.m_cv.is_const,
                            other.m_cv.is_volatile);
        }

        bool operator<=(const type_identifier& other) const
        {
            return std::tie(m_type_index,
                            m_kind,
                            m_cv.is_const,
                            m_cv.is_volatile) <=
                   std::tie(other.m_type_index,
                            other.m_kind,
                            other.m_cv.is_const,
                            other.m_cv.is_volatile);
        }

        bool operator>=(const type_identifier& other) const
        {
            return std::tie(m_type_index,
                            m_kind,
                            m_cv.is_const,
                            m_cv.is_volatile) >=
                   std::tie(other.m_type_index,
                            other.m_kind,
                            other.m_cv.is_const,
                            other.m_cv.is_volatile);
        }

    private:
        std::type_index m_type_index;
        kind_type       m_kind;

        struct type_const_volatile
        {
            uint8_t is_const : 1;
            uint8_t is_volatile : 1;
        };
        type_const_volatile m_cv{0, 0};
    };

    /**
     * @brief Kind of a type.
     * @tparam T type
     * @return kind of type
     */
    template <typename T>
    constexpr type_identifier::type_identifier::kind_type type_kind()
    {
        if constexpr (std::is_void_v<T>) {
            return type_identifier::TYPE_VOID;
        } else if constexpr (std::is_same<bool, T>::value ||
                             std::is_integral<T>::value ||
                             std::is_floating_point<T>::value) {
            return type_identifier::TYPE_POD;
        } else if constexpr (std::is_enum_v<T>) {
            return type_identifier::TYPE_ENUM;
        } else if constexpr (std::is_class_v<T>) {
            return type_identifier::TYPE_CLASS;
        } else if constexpr (std::is_pointer_v<T>) {
            return type_identifier::TYPE_POINTER;
        } else if constexpr (std::is_reference_v<T> &&
                             !std::is_rvalue_reference_v<T>) {
            return type_identifier::TYPE_REFERENCE;
        } else if constexpr (std::is_rvalue_reference_v<T>) {
            return type_identifier::TYPE_RVALUE_REFERENCE;
        } else {
            static_assert(false, "Unknown type kind");
        }
    }

    /**
     * @brief Create a type identifier for a type.
     */
    template <typename T> constexpr type_identifier make_type_identifier()
    {
        if constexpr (std::is_const_v<T>) {
            if constexpr (std::is_volatile_v<T>) {
                return type_identifier(typeid(T), type_kind<T>(), true, true);
            } else {
                return type_identifier(typeid(T), type_kind<T>(), true, false);
            }
        } else {
            if constexpr (std::is_volatile_v<T>) {
                return type_identifier(typeid(T), type_kind<T>(), false, true);
            } else {
                return type_identifier(typeid(T), type_kind<T>(), false, false);
            }
        }
    }

    inline std::ostream& operator<<(std::ostream& os, const type_identifier& id)
    {
        if (id.is_const()) {
            os << "const ";
        }
        if (id.is_volatile()) {
            os << "volatile ";
        }
        os << id.type_index().name();
        switch (id.kind()) {
        case type_identifier::TYPE_POINTER:
            break;
        case type_identifier::TYPE_REFERENCE:
            os << "&";
            break;
        case type_identifier::TYPE_RVALUE_REFERENCE:
            os << "&&";
            break;
        default:
            break;
        }
        return os;
    }
} // namespace mge::script
