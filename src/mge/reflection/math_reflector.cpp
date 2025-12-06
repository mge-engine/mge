// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/math/vec2.hpp"
#include "mge/math/vec3.hpp"
#include "mge/math/vec4.hpp"

namespace mge::reflection {

    class math_reflector : public reflector
    {
    public:
        math_reflector() = default;
        ~math_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(type<mge::fvec2>()
                    .field("x", &mge::fvec2::x)
                    .field("y", &mge::fvec2::y));

            mge(type<mge::fvec3>()
                    .field("x", &mge::fvec3::x)
                    .field("y", &mge::fvec3::y)
                    .field("z", &mge::fvec3::z));

            mge(type<mge::fvec4>()
                    .field("x", &mge::fvec4::x)
                    .field("y", &mge::fvec4::y)
                    .field("z", &mge::fvec4::z)
                    .field("w", &mge::fvec4::w));
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 2> deps{{"core", "std"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(math_reflector,
                                mge::reflection::reflector,
                                math);
} // namespace mge::reflection
