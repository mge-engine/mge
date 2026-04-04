// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"

#include <filesystem>

namespace mge::reflection {

    class std_reflector : public reflector
    {
    public:
        std_reflector() = default;
        ~std_reflector() = default;

        void reflect() const override
        {
            auto std = module("std");
            std(function("terminate", &std::terminate));

            auto std_filesystem = module("std::filesystem");

            std_filesystem(
                type<std::filesystem::path>().constructor<std::string_view>());
        }
    };

    MGE_REGISTER_IMPLEMENTATION(std_reflector, mge::reflection::reflector, std);

} // namespace mge::reflection
