// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"
#include "mge/script/script_fwd.hpp"
#include "mge/script/type_details.hpp"

#include "lua.hpp"
#include "lua_context.hpp"

namespace mge::lua {
    MGE_DECLARE_REF(type);

    class type
    {
    public:
        type(lua_context&                         context,
             const mge::script::type_details_ref& details);
        ~type();

        const mge::script::type_details& details() const { return *m_details; }

        void add_enum_value(const std::string& name, int64_t enum_value);

    private:
        struct create_data
        {
            std::map<std::string, int64_t> enum_values;
        };

        mutable std::unique_ptr<create_data> m_create_data;
        const mge::script::type_details_ref& m_details;
    };
} // namespace mge::lua