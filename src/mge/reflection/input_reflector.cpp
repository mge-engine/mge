// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/input/key.hpp"
#include "mge/input/key_action.hpp"
#include "mge/input/mouse_action.hpp"

namespace mge::reflection {

    class input_reflector : public reflector
    {
    public:
        input_reflector() = default;
        ~input_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(type<mge::key_action>()
                    .value("PRESS", mge::key_action::PRESS)
                    .value("RELEASE", mge::key_action::RELEASE)
                    .value("REPEAT", mge::key_action::REPEAT));

            mge(type<mge::mouse_action>()
                    .value("PRESS", mge::mouse_action::PRESS)
                    .value("RELEASE", mge::mouse_action::RELEASE)
                    .value("DOUBLE_CLICK",
                           mge::mouse_action::MOUSE_ACTION_DOUBLE_CLICK));

            mge(type<mge::key>());
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 2> deps{{"core", "std"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(input_reflector,
                                mge::reflection::reflector,
                                input);
} // namespace mge::reflection
