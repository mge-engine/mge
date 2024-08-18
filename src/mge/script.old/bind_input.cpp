// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/input_handler.hpp"
#include "mge/input/input_state.hpp"
#include "mge/input/key.hpp"
#include "mge/input/keyboard_state.hpp"
#include "mge/input/modifier.hpp"
#include "mge/input/mouse_action.hpp"
#include "mge/input/mouse_state.hpp"

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
#if 0
            module("mge")(
                // module begin
                type<mge::input_handler>("input_handler")(
                    type<mge::input_handler::mouse_action_handler>(
                        "mouse_action_handler"),
                    type<mge::input_handler::mouse_move_handler>(
                        "mouse_move_handler"),
                    type<mge::input_handler::key_action_handler>(
                        "key_action_handler"),
                    type<mge::input_handler::character_handler>(
                        "character_handler"))
                    .method("set_key_action_handler",
                            &mge::input_handler::set_key_action_handler)
                    .method("set_character_handler",
                            &mge::input_handler::set_character_handler),
                type<mge::input_state>("input_state")
                    .constructor()
                    .method("keyboard",
                            static_cast<const mge::keyboard_state& (
                                mge::input_state::*)() const>(
                                &mge::input_state::keyboard))
                    .method("mouse",
                            static_cast<const mge::mouse_state& (
                                mge::input_state::*)() const>(
                                &mge::input_state::mouse)),
                type<mge::keyboard_state>("keyboard_state")
                    .constructor()
                    .method(
                        "press",
                        static_cast<void (mge::keyboard_state::*)(
                            const mge::modifier&)>(&mge::keyboard_state::press))
                    .method("release",
                            static_cast<void (mge::keyboard_state::*)(
                                const mge::modifier&)>(
                                &mge::keyboard_state::release))
                    .method("press",
                            static_cast<void (mge::keyboard_state::*)(
                                const mge::key&)>(&mge::keyboard_state::press))
                    .method(
                        "release",
                        static_cast<void (mge::keyboard_state::*)(
                            const mge::key&)>(&mge::keyboard_state::release))
                    .method("pressed",
                            static_cast<bool (mge::keyboard_state::*)(
                                const mge::key&) const>(
                                &mge::keyboard_state::pressed))
                    .method("pressed",
                            static_cast<bool (mge::keyboard_state::*)(
                                const mge::modifier&) const>(
                                &mge::keyboard_state::pressed))
                    .method("pressed",
                            static_cast<bool (mge::keyboard_state::*)(
                                const mge::modifier&,
                                const mge::key&) const>(
                                &mge::keyboard_state::pressed))
                    .method("current_modifier",
                            &mge::keyboard_state::current_modifier),
                type<mge::mouse_state>("mouse_state")
                    .constructor()
                    .method("press", &mge::mouse_state::press)
                    .method("release", &mge::mouse_state::release)
                    .method("move", &mge::mouse_state::move)
                    .method("x", &mge::mouse_state::x)
                    .method("y", &mge::mouse_state::y)
                    .method("pressed", &mge::mouse_state::pressed),
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
#endif
        }
    };

    MGE_REGISTER_IMPLEMENTATION(input_script_binder,
                                mge::script_binder,
                                mgeinput,
                                input);

} // namespace mge::script