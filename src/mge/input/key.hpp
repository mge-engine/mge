// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/core/enum.hpp"
#include "mge/input/dllexport.hpp"
#include <cstdint>
#include <iosfwd>

namespace mge {

    /**
     * @brief Key constants used in keyboard press events.
     *
     * Values are aligned 1:1 with GLFW key constants.
     */
    enum class MGEINPUT_EXPORT key : int32_t
    {
        INVALID = -1, //!< INVALID

        SPACE = 32,      //!< SPACE
        APOSTROPHE = 39, //!< APOSTROPHE '
        COMMA = 44,      //!< COMMA ,
        MINUS = 45,      //!< MINUS -
        PERIOD = 46,     //!< PERIOD .
        SLASH = 47,      //!< SLASH /

        ZERO = 48, //!< ZERO
        ONE,       //!< ONE
        TWO,       //!< TWO
        THREE,     //!< THREE
        FOUR,      //!< FOUR
        FIVE,      //!< FIVE
        SIX,       //!< SIX
        SEVEN,     //!< SEVEN
        EIGHT,     //!< EIGHT
        NINE,      //!< NINE

        SEMICOLON = 59, //!< SEMICOLON ;
        EQUAL = 61,     //!< EQUAL =

        A = 65, //!< A
        B,      //!< B
        C,      //!< C
        D,      //!< D
        E,      //!< E
        F,      //!< F
        G,      //!< G
        H,      //!< H
        I,      //!< I
        J,      //!< J
        K,      //!< K
        L,      //!< L
        M,      //!< M
        N,      //!< N
        O,      //!< O
        P,      //!< P
        Q,      //!< Q
        R,      //!< R
        S,      //!< S
        T,      //!< T
        U,      //!< U
        V,      //!< V
        W,      //!< W
        X,      //!< X
        Y,      //!< Y
        Z,      //!< Z

        LEFT_BRACKET = 91,  //!< LEFT_BRACKET [
        BACKSLASH = 92,     //!< BACKSLASH
        RIGHT_BRACKET = 93, //!< RIGHT_BRACKET ]
        GRAVE_ACCENT = 96,  //!< GRAVE_ACCENT `

        WORLD_1 = 161, //!< WORLD_1 non-US #1
        WORLD_2 = 162, //!< WORLD_2 non-US #2

        ESCAPE = 256,    //!< ESCAPE
        ENTER = 257,     //!< ENTER
        TAB = 258,       //!< TAB
        BACKSPACE = 259, //!< BACKSPACE

        INSERT = 260,       //!< INSERT
        DELETE_KEY = 261,   //!< DELETE_KEY
        CURSOR_RIGHT = 262, //!< CURSOR_RIGHT
        CURSOR_LEFT = 263,  //!< CURSOR_LEFT
        CURSOR_DOWN = 264,  //!< CURSOR_DOWN
        CURSOR_UP = 265,    //!< CURSOR_UP

        PAGE_UP = 266, //!< PAGE_UP
        PRIOR = PAGE_UP,
        PAGE_DOWN = 267, //!< PAGE_DOWN
        NEXT = PAGE_DOWN,
        HOME = 268, //!< HOME
        END = 269,  //!< END

        CAPS_LOCK = 280, //!< CAPS_LOCK
        CAPITAL = CAPS_LOCK,
        SCROLL_LOCK = 281,  //!< SCROLL_LOCK
        NUM_LOCK = 282,     //!< NUM_LOCK
        PRINT_SCREEN = 283, //!< PRINT_SCREEN
        PAUSE = 284,        //!< PAUSE

        F1 = 290, //!< F1
        F2,       //!< F2
        F3,       //!< F3
        F4,       //!< F4
        F5,       //!< F5
        F6,       //!< F6
        F7,       //!< F7
        F8,       //!< F8
        F9,       //!< F9
        F10,      //!< F10
        F11,      //!< F11
        F12,      //!< F12
        F13,      //!< F13
        F14,      //!< F14
        F15,      //!< F15
        F16,      //!< F16
        F17,      //!< F17
        F18,      //!< F18
        F19,      //!< F19
        F20,      //!< F20
        F21,      //!< F21
        F22,      //!< F22
        F23,      //!< F23
        F24,      //!< F24
        F25,      //!< F25

        KP_0 = 320,        //!< KP_0
        KP_1,              //!< KP_1
        KP_2,              //!< KP_2
        KP_3,              //!< KP_3
        KP_4,              //!< KP_4
        KP_5,              //!< KP_5
        KP_6,              //!< KP_6
        KP_7,              //!< KP_7
        KP_8,              //!< KP_8
        KP_9,              //!< KP_9
        KP_DECIMAL = 330,  //!< KP_DECIMAL
        KP_DIVIDE = 331,   //!< KP_DIVIDE
        KP_MULTIPLY = 332, //!< KP_MULTIPLY
        KP_SUBTRACT = 333, //!< KP_SUBTRACT
        KP_ADD = 334,      //!< KP_ADD
        KP_ENTER = 335,    //!< KP_ENTER
        KP_EQUAL = 336,    //!< KP_EQUAL

        LEFT_SHIFT = 340,   //!< LEFT_SHIFT
        LEFT_CONTROL = 341, //!< LEFT_CONTROL
        LEFT_ALT = 342,     //!< LEFT_ALT
        LEFT_SUPER = 343,   //!< LEFT_SUPER

        RIGHT_SHIFT = 344,   //!< RIGHT_SHIFT
        RIGHT_CONTROL = 345, //!< RIGHT_CONTROL
        RIGHT_ALT = 346,     //!< RIGHT_ALT
        RIGHT_SUPER = 347,   //!< RIGHT_SUPER

        MENU = 348, //!< MENU

        KEY_MAX //!< maximum possible key
    };
} // namespace mge

template <> struct mge::enum_range<mge::key>
{
    static constexpr int min = static_cast<int>(mge::key::INVALID);
    static constexpr int max = static_cast<int>(mge::key::KEY_MAX);
};
