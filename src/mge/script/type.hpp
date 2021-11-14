// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/sfinae.hpp"
#include "mge/core/type_name.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_details.hpp"
#include "mge/script/visitor.hpp"

#include <list>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace mge::script {

    namespace details {
        class MGESCRIPT_EXPORT type_base
        {
        public:
            type_base() = default;
            virtual ~type_base() = default;

            const std::string&     name() const;
            mge::script::module    module() const;
            const type_details_ref details() const;

        protected:
            void get_or_create_details(const std::string& name);

            type_details_ref m_details;
        };

    } // namespace details

    template <typename T, typename = void>
    class type : public details::type_base
    {
    public:
        inline type() { get_or_create_details(mge::type_name<T>()); }

        using details::type_base::details;
        using details::type_base::name;
    };

    template <> class type<void, void> : public details::type_base
    {
    public:
        inline type() { get_or_create_details("void"); }

        using details::type_base::details;
        using details::type_base::name;
    };

} // namespace mge::script