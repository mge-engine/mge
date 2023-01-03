// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/input/key.hpp"
#include <iostream>

namespace mge {

    std::ostream& operator<<(std::ostream& os, const key& k)
    {
        switch (k) {
#define CASE(K)                                                                \
    case key::K:                                                               \
        return os << #K
            CASE(INVALID);
            CASE(SPACE);
            CASE(ZERO);
            CASE(ONE);
            CASE(TWO);
            CASE(THREE);
            CASE(FOUR);
            CASE(FIVE);
            CASE(SIX);
            CASE(SEVEN);
            CASE(EIGHT);
            CASE(NINE);
            CASE(A);
            CASE(B);
            CASE(C);
            CASE(D);
            CASE(E);
            CASE(F);
            CASE(G);
            CASE(H);
            CASE(I);
            CASE(J);
            CASE(K);
            CASE(L);
            CASE(M);
            CASE(N);
            CASE(O);
            CASE(P);
            CASE(Q);
            CASE(R);
            CASE(S);
            CASE(T);
            CASE(U);
            CASE(V);
            CASE(W);
            CASE(X);
            CASE(Y);
            CASE(Z);
            CASE(ESCAPE);
            CASE(ENTER);
            CASE(BACKSPACE);
            CASE(TAB);
            CASE(F1);
            CASE(F2);
            CASE(F3);
            CASE(F4);
            CASE(F5);
            CASE(F6);
            CASE(F7);
            CASE(F8);
            CASE(F9);
            CASE(F10);
            CASE(F11);
            CASE(F12);
            CASE(INSERT);
            CASE(DELETE_KEY);
            CASE(HOME);
            CASE(END);
            CASE(PAGE_UP);
            CASE(PAGE_DOWN);
            CASE(CURSOR_LEFT);
            CASE(CURSOR_RIGHT);
            CASE(CURSOR_UP);
            CASE(CURSOR_DOWN);
            CASE(CAPS_LOCK);
            CASE(SCROLL_LOCK);
            CASE(PAUSE);
            CASE(PRINT_SCREEN);
            CASE(NUM_LOCK);
            CASE(LEFT_SHIFT);
            CASE(LEFT_CONTROL);
            CASE(LEFT_ALT);
            CASE(RIGHT_SHIFT);
            CASE(RIGHT_CONTROL);
            CASE(RIGHT_ALT);
            CASE(MENU);
            CASE(KP_0);
            CASE(KP_1);
            CASE(KP_2);
            CASE(KP_3);
            CASE(KP_4);
            CASE(KP_5);
            CASE(KP_6);
            CASE(KP_7);
            CASE(KP_8);
            CASE(KP_9);
            CASE(KP_ADD);
            CASE(KP_SUBTRACT);
            CASE(KP_MULTIPLY);
            CASE(KP_DIVIDE);
            CASE(KP_DECIMAL);
            CASE(KP_ENTER);
#undef CASE
        default:
            return os << "INVALID(" << static_cast<int>(k) << ")";
        }
    }
} // namespace mge
