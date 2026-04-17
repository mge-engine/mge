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

            mge(type<mge::key>()
                    .value("INVALID", mge::key::INVALID)
                    .value("SPACE", mge::key::SPACE)
                    .value("ZERO", mge::key::ZERO)
                    .value("ONE", mge::key::ONE)
                    .value("TWO", mge::key::TWO)
                    .value("THREE", mge::key::THREE)
                    .value("FOUR", mge::key::FOUR)
                    .value("FIVE", mge::key::FIVE)
                    .value("SIX", mge::key::SIX)
                    .value("SEVEN", mge::key::SEVEN)
                    .value("EIGHT", mge::key::EIGHT)
                    .value("NINE", mge::key::NINE)
                    .value("A", mge::key::A)
                    .value("B", mge::key::B)
                    .value("C", mge::key::C)
                    .value("D", mge::key::D)
                    .value("E", mge::key::E)
                    .value("F", mge::key::F)
                    .value("G", mge::key::G)
                    .value("H", mge::key::H)
                    .value("I", mge::key::I)
                    .value("J", mge::key::J)
                    .value("K", mge::key::K)
                    .value("L", mge::key::L)
                    .value("M", mge::key::M)
                    .value("N", mge::key::N)
                    .value("O", mge::key::O)
                    .value("P", mge::key::P)
                    .value("Q", mge::key::Q)
                    .value("R", mge::key::R)
                    .value("S", mge::key::S)
                    .value("T", mge::key::T)
                    .value("U", mge::key::U)
                    .value("V", mge::key::V)
                    .value("W", mge::key::W)
                    .value("X", mge::key::X)
                    .value("Y", mge::key::Y)
                    .value("Z", mge::key::Z)
                    .value("ESCAPE", mge::key::ESCAPE)
                    .value("ENTER", mge::key::ENTER)
                    .value("TAB", mge::key::TAB)
                    .value("BACKSPACE", mge::key::BACKSPACE)
                    .value("F1", mge::key::F1)
                    .value("F2", mge::key::F2)
                    .value("F3", mge::key::F3)
                    .value("F4", mge::key::F4)
                    .value("F5", mge::key::F5)
                    .value("F6", mge::key::F6)
                    .value("F7", mge::key::F7)
                    .value("F8", mge::key::F8)
                    .value("F9", mge::key::F9)
                    .value("F10", mge::key::F10)
                    .value("F11", mge::key::F11)
                    .value("F12", mge::key::F12)
                    .value("F13", mge::key::F13)
                    .value("F14", mge::key::F14)
                    .value("F15", mge::key::F15)
                    .value("F16", mge::key::F16)
                    .value("F17", mge::key::F17)
                    .value("F18", mge::key::F18)
                    .value("F19", mge::key::F19)
                    .value("F20", mge::key::F20)
                    .value("F21", mge::key::F21)
                    .value("F22", mge::key::F22)
                    .value("F23", mge::key::F23)
                    .value("F24", mge::key::F24)
                    .value("F25", mge::key::F25)
                    .value("INSERT", mge::key::INSERT)
                    .value("DELETE_KEY", mge::key::DELETE_KEY)
                    .value("HOME", mge::key::HOME)
                    .value("END", mge::key::END)
                    .value("PAGE_UP", mge::key::PAGE_UP)
                    .value("PAGE_DOWN", mge::key::PAGE_DOWN)
                    .value("CURSOR_UP", mge::key::CURSOR_UP)
                    .value("CURSOR_DOWN", mge::key::CURSOR_DOWN)
                    .value("CURSOR_LEFT", mge::key::CURSOR_LEFT)
                    .value("CURSOR_RIGHT", mge::key::CURSOR_RIGHT)
                    .value("CAPS_LOCK", mge::key::CAPS_LOCK)
                    .value("SCROLL_LOCK", mge::key::SCROLL_LOCK)
                    .value("NUM_LOCK", mge::key::NUM_LOCK)
                    .value("PRINT_SCREEN", mge::key::PRINT_SCREEN)
                    .value("PAUSE", mge::key::PAUSE)
                    .value("LEFT_SHIFT", mge::key::LEFT_SHIFT)
                    .value("LEFT_CONTROL", mge::key::LEFT_CONTROL)
                    .value("LEFT_ALT", mge::key::LEFT_ALT)
                    .value("LEFT_SUPER", mge::key::LEFT_SUPER)
                    .value("RIGHT_SHIFT", mge::key::RIGHT_SHIFT)
                    .value("RIGHT_CONTROL", mge::key::RIGHT_CONTROL)
                    .value("RIGHT_ALT", mge::key::RIGHT_ALT)
                    .value("RIGHT_SUPER", mge::key::RIGHT_SUPER)
                    .value("MENU", mge::key::MENU)
                    .value("APOSTROPHE", mge::key::APOSTROPHE)
                    .value("COMMA", mge::key::COMMA)
                    .value("MINUS", mge::key::MINUS)
                    .value("PERIOD", mge::key::PERIOD)
                    .value("SLASH", mge::key::SLASH)
                    .value("SEMICOLON", mge::key::SEMICOLON)
                    .value("EQUAL", mge::key::EQUAL)
                    .value("LEFT_BRACKET", mge::key::LEFT_BRACKET)
                    .value("BACKSLASH", mge::key::BACKSLASH)
                    .value("RIGHT_BRACKET", mge::key::RIGHT_BRACKET)
                    .value("GRAVE_ACCENT", mge::key::GRAVE_ACCENT)
                    .value("KP_0", mge::key::KP_0)
                    .value("KP_1", mge::key::KP_1)
                    .value("KP_2", mge::key::KP_2)
                    .value("KP_3", mge::key::KP_3)
                    .value("KP_4", mge::key::KP_4)
                    .value("KP_5", mge::key::KP_5)
                    .value("KP_6", mge::key::KP_6)
                    .value("KP_7", mge::key::KP_7)
                    .value("KP_8", mge::key::KP_8)
                    .value("KP_9", mge::key::KP_9)
                    .value("KP_DECIMAL", mge::key::KP_DECIMAL)
                    .value("KP_DIVIDE", mge::key::KP_DIVIDE)
                    .value("KP_MULTIPLY", mge::key::KP_MULTIPLY)
                    .value("KP_SUBTRACT", mge::key::KP_SUBTRACT)
                    .value("KP_ADD", mge::key::KP_ADD)
                    .value("KP_ENTER", mge::key::KP_ENTER)
                    .value("KP_EQUAL", mge::key::KP_EQUAL));
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
