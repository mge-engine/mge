// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

#include <exception>

namespace mge::script {
    class std_script_binder : public script_binder
    {
    public:
        std_script_binder() = default;
        ~std_script_binder() = default;

        void bind() override
        {
            mge::script::module std("std");
            std(type<std::exception>().method("what", &std::exception::what),
                function("terminate", &std::terminate));
        }
    };

    MGE_REGISTER_IMPLEMENTATION(std_script_binder, mge::script_binder, std);

} // namespace mge::script