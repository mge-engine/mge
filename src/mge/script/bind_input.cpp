// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/input/key.hpp"
#include "mge/input/keyboard_state.hpp"
#include "mge/input/modifier.hpp"
#include "mge/input/mouse_action.hpp"

#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {

    class input_script_binder : public script_binder
    {
    public:
        input_script_binder() = default;

        void bind()
        {
            module("mge")(
                // module begin
                type<mge::keyboard_state>("keyboard_state"),
                type<mge::key>("key")
                    .enum_value("INVALID", mge::key::INVALID)
                    .enum_value("SPACE", mge::key::SPACE)
                    .enum_value("ZERO", mge::key::ZERO),
                type<mge::modifier>("modifier"),
                type<mge::modifier_value>("modifier_value")
                    .enum_value("CONTROL", mge::modifier_value::CONTROL)
                    .enum_value("SHIFT", mge::modifier_value::SHIFT)
                    .enum_value("ALT", mge::modifier_value::ALT),
                type<mge::mouse_action>("mouse_action")
                    .enum_value("PRESS", mge::mouse_action::PRESS)
                    .enum_value("RELEASE", mge::mouse_action::RELEASE)
                    .enum_value("DOUBLE_CLICK",
                                mge::mouse_action::MOUSE_ACTION_DOUBLE_CLICK)
                // module end
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(input_script_binder,
                                mge::script_binder,
                                mgeinput,
                                input);

} // namespace mge::script