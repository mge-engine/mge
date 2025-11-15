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
        bool                    is_class = false;
        bool                    is_pointer = false;
        bool                    is_array = false;
        size_t                  size = 0;
        std::string_view        name;
        module_details_weak_ref module;

        struct enum_specific_details
        {
            type_details_ref underlying_type;
            using value_type = std::variant<int64_t, uint64_t>;
            std::vector<std::pair<std::string_view, value_type>> values;
        };

        struct class_specific_details
        {
            std::vector<type_details_ref> bases;

            void add_base(const type_details_ref& base)
            {
                auto it = std::find(bases.begin(), bases.end(), base);
                if (it != bases.end()) {
                    return;
                }
                bases.push_back(base);
            }
        };

        struct pointer_specific_details
        {
            type_details_ref element_type;
        };

        struct array_specific_details
        {
            type_details_ref element_type;
            size_t           size = 0; // 0 means unknown size
        };

        std::variant<std::monostate,
                     enum_specific_details,
                     class_specific_details,
                     pointer_specific_details,
                     array_specific_details>
            specific_details;

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

        class_specific_details& class_specific()
        {
            if (!is_class) {
                MGE_THROW(illegal_state)
                    << "Type " << name << " is not a class";
            }

            if (!std::holds_alternative<class_specific_details>(
                    specific_details)) {
                specific_details = class_specific_details{};
            }
            return std::get<class_specific_details>(specific_details);
        }

        pointer_specific_details& pointer_specific()
        {
            if (!std::holds_alternative<pointer_specific_details>(
                    specific_details)) {
                specific_details = pointer_specific_details{};
            }
            return std::get<pointer_specific_details>(specific_details);
        }

        array_specific_details& array_specific()
        {
            if (!is_array) {
                MGE_THROW(illegal_state)
                    << "Type " << name << " is not an array";
            }

            if (!std::holds_alternative<array_specific_details>(
                    specific_details)) {
                specific_details = array_specific_details{};
            }
            return std::get<array_specific_details>(specific_details);
        }
    };

} // namespace mge::reflection
