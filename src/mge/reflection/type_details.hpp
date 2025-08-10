// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/reflection/dllexport.hpp"
#include "mge/reflection/reflection_fwd.hpp"
#include "mge/reflection/type_identifier.hpp"

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

        bool   is_void = false;
        bool   is_bool = false;
        bool   is_integral = false;
        size_t size = 0;
    };

} // namespace mge::reflection
