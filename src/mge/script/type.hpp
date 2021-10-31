// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"

#include <list>
#include <string_view>
#include <tuple>

namespace mge::script {

    class MGESCRIPT_EXPORT type
    {
    protected:
        type(const std::string_view name);

    public:
        virtual ~type();
        std::string_view name() const;

        virtual void apply(visitor& v);

    private:
        std::string m_name;
    };

    template <typename T> class MGESCRIPT_EXPORT enum_type : type
    {
    protected:
        enum_type(const std::string_view name);

    public:
        using self_type = enum_type<T>;
        virtual ~enum_type();

        virtual void apply(visitor& v);

    private:
        using enum_value_type = std::tuple<std::string, T>;
        std::list<enum_value_type> m_values;
    };

} // namespace mge::script