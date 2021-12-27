// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/script/call_context.hpp"
#include "mge/script/script_fwd.hpp"

#include <string>
#include <typeindex>

namespace mge::script {
    struct field_details
    {
        field_details(const std::string& name_,
                      std::type_index    type_,
                      context_function&& setter_,
                      context_function&& getter_)
            : name(name_)
            , type(type_)
            , getter(std::move(getter_))
            , setter(std::move(setter_))
        {}

        field_details(const field_details&) = default;
        field_details(field_details&&) = default;
        field_details& operator=(const field_details&) = default;
        field_details& operator=(field_details&&) = default;

        std::string      name;
        std::type_index  type;
        context_function getter;
        context_function setter;
    };

} // namespace mge::script