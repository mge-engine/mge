#pragma once
#include "mge/input/dllexport.hpp"
#include <iosfwd>
namespace mge {

    enum class MGE_CORE_EXPORT key
    {
        INVALID = -1, //!< INVALID

        SPACE = 32,   //!< SPACE

        ZERO = 48,    //!< ZERO
        ONE,          //!< ONE
        TWO,          //!< TWO
        THREE,        //!< THREE
        FOUR,         //!< FOUR
        FIVE,         //!< FIVE
        SIX,          //!< SIX
        SEVEN,        //!< SEVEN
        EIGHT,        //!< EIGHT
        NINE,         //!< NINE

        A = 65,       //!< A
        B,            //!< B
        C,            //!< C
        D,            //!< D
        E,            //!< E
        F,            //!< F
        G,            //!< G
        H,            //!< H
        I,            //!< I
        J,            //!< J
        K,            //!< K
        L,            //!< L
        M,            //!< M
        N,            //!< N
        O,            //!< O
        P,            //!< P
        Q,            //!< Q
        R,            //!< R
        S,            //!< S
        T,            //!< T
        U,            //!< U
        V,            //!< V
        W,            //!< W
        X,            //!< X
        Y,            //!< Y
        Z,            //!< Z

        ESCAPE = 256, //!< ESCAPE
        ENTER,        //!< ENTER
        BACKSPACE,    //!< BACKSPACE
        TAB,          //!< TAB

        F1,           //!< F1
        F2,           //!< F2
        F3,           //!< F3
        F4,           //!< F4
        F5,           //!< F5
        F6,           //!< F6
        F7,           //!< F7
        F8,           //!< F8
        F9,           //!< F9
        F10,          //!< F10
        F11,          //!< F11
        F12,          //!< F12

        INSERT,       //!< INSERT
        DELETE_KEY,   //!< DELETE_KEY
        HOME,         //!< HOME
        END,          //!< END
        PAGE_UP,      //!< PAGE_UP
        PRIOR = PAGE_UP,
        PAGE_DOWN,    //!< PAGE_DOWN
        NEXT = PAGE_DOWN,

        CURSOR_UP,    //!< CURSOR_UP
        CURSOR_DOWN,  //!< CURSOR_DOWN
        CURSOR_LEFT,  //!< CURSOR_LEFT
        CURSOR_RIGHT, //!< CURSOR_RIGHT

        CAPS_LOCK,    //!< CAPS_LOCK
        CAPITAL = CAPS_LOCK,
        SCROLL_LOCK,  //!< SCROLL_LOCK
        PAUSE,        //!< PAUSE
        PRINT_SCREEN, //!< PRINT_SCREEN
        NUM_LOCK,     //!< NUM_LOCK

        LEFT_SHIFT,   //!< LEFT_SHIFT
        LEFT_CONTROL, //!< LEFT_CONTROL
        LEFT_ALT,     //!< LEFT_ALT

        RIGHT_SHIFT,  //!< RIGHT_SHIFT
        RIGHT_CONTROL,//!< RIGHT_CONTROL
        RIGHT_ALT,    //!< RIGHT_ALT

        MENU,         //!< MENU

        KP_0,         //!< KP_0
        KP_1,         //!< KP_1
        KP_2,         //!< KP_2
        KP_3,         //!< KP_3
        KP_4,         //!< KP_4
        KP_5,         //!< KP_5
        KP_6,         //!< KP_6
        KP_7,         //!< KP_7
        KP_8,         //!< KP_8
        KP_9,         //!< KP_9
        KP_ADD,       //!< KP_ADD
        KP_SUBTRACT,  //!< KP_SUBTRACT
        KP_MULTIPLY,  //!< KP_MULTIPLY
        KP_DIVIDE,    //!< KP_DIVIDE
        KP_DECIMAL,   //!< KP_DECIMAL
        KP_ENTER,     //!< KP_ENTER

        KEY_MAX       //!< maximum possible key
    };

    MGE_INPUT_EXPORT std::ostream& operator <<(std::ostream& os, const key& k);
}
