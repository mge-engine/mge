// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/traits.hpp"
#include <string>

namespace mge::script {

    class MGESCRIPT_EXPORT type_base
    {
    public:
        const std::string&     name() const;
        mge::script::module    module() const;
        const std::type_index& type_index() const;
        type_details*          details() const;

    protected:
        void init_details(const std::type_index& index);
        void init_enum_details(const std::type_index& index,
                               const std::string&     name,
                               const traits&          tr);

    private:
        type_details* m_details;
    };

    template <typename T, typename = void> class type : public type_base
    {
    public:
        inline type()
        {
            static_assert(false, "Default expansion of type detected");
        }
    };

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
        inline explicit type()
        {
            auto ti = std::type_index(typeid(T));
            auto n = mge::base_type_name<T>();
            auto tr = traits_of<T>();
            init_enum_details(ti, n, tr);
        }

        using type_base::details;
        using type_base::module;
        using type_base::name;
        using type_base::type_index;
    };

} // namespace mge::script