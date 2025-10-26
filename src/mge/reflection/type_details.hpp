// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/stdexceptions.hpp"
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/type_identifier.hpp"

#include <variant>

namespace mge::reflection {

    class MGEREFLECTION_EXPORT type_details
    {
    public:
        type_details();
        ~type_details();

        type_details(const type_details&) = delete;
        type_details& operator=(const type_details&) = delete;
        type_details(type_details&&) = delete;
        type_details& operator=(type_details&&) = delete;

        template <typename T> static const type_details_ref& get()
        {
            return get(make_type_identifier<T>());
        }

        static const type_details_ref& get(const type_identifier& id);
        static const type_details_ref& put(const type_identifier&  id,
                                           const type_details_ref& details);

        bool                    is_void = false;
        bool                    is_bool = false;
        bool                    is_integral = false;
        bool                    is_floating_point = false;
        bool                    is_enum = false;
        size_t                  size = 0;
        std::string_view        name;
        module_details_weak_ref module;

        struct enum_specific_details
        {
            type_details_ref underlying_type;
        };

        std::variant<std::monostate, enum_specific_details> specific_details;

        enum_specific_details& enum_specific()
        {
            if (!is_enum) {
                MGE_THROW(illegal_state)
                    << "Type " << name << " is not an enum";
            }

            if (!std::holds_alternative<enum_specific_details>(
                    specific_details)) {
                specific_details = enum_specific_details{};
            }
            return std::get<enum_specific_details>(specific_details);
        }
    };

} // namespace mge::reflection
