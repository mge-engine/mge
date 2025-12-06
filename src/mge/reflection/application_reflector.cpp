// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/application/application.hpp"

namespace mge::reflection {

    class application_reflector : public reflector
    {
    public:
        application_reflector() = default;
        ~application_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(type<mge::application>());
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 2> deps{{"core", "std"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(application_reflector,
                                mge::reflection::reflector,
                                application);
} // namespace mge::reflection
