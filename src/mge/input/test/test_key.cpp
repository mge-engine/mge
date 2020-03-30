// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/input/key.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(key, print)
{
#define CHECK_KEY(K) mge::test_stream_output(mge::key::K, #K);
    CHECK_KEY(INVALID);
    CHECK_KEY(SPACE);
    CHECK_KEY(ZERO);
    CHECK_KEY(ONE);
    CHECK_KEY(TWO);
    CHECK_KEY(THREE);
    CHECK_KEY(FOUR);
    CHECK_KEY(FIVE);
    CHECK_KEY(SIX);
    CHECK_KEY(SEVEN);
    CHECK_KEY(EIGHT);
    CHECK_KEY(NINE);
    CHECK_KEY(A);
    CHECK_KEY(B);
    CHECK_KEY(C);
    CHECK_KEY(D);
    CHECK_KEY(E);
    CHECK_KEY(F);
    CHECK_KEY(G);
    CHECK_KEY(H);
    CHECK_KEY(I);
    CHECK_KEY(J);
    CHECK_KEY(K);
    CHECK_KEY(L);
    CHECK_KEY(M);
    CHECK_KEY(N);
    CHECK_KEY(O);
    CHECK_KEY(P);
    CHECK_KEY(Q);
    CHECK_KEY(R);
    CHECK_KEY(S);
    CHECK_KEY(T);
    CHECK_KEY(U);
    CHECK_KEY(V);
    CHECK_KEY(W);
    CHECK_KEY(X);
    CHECK_KEY(Y);
    CHECK_KEY(Z);
    CHECK_KEY(ESCAPE);
    CHECK_KEY(ENTER);
    CHECK_KEY(BACKSPACE);
    CHECK_KEY(TAB);
    CHECK_KEY(F1);
    CHECK_KEY(F2);
    CHECK_KEY(F3);
    CHECK_KEY(F4);
    CHECK_KEY(F5);
    CHECK_KEY(F6);
    CHECK_KEY(F7);
    CHECK_KEY(F8);
    CHECK_KEY(F9);
    CHECK_KEY(F10);
    CHECK_KEY(F11);
    CHECK_KEY(F12);
    CHECK_KEY(INSERT);
    CHECK_KEY(DELETE_KEY);
    CHECK_KEY(HOME);
    CHECK_KEY(END);
    CHECK_KEY(PAGE_UP);
    CHECK_KEY(PAGE_DOWN);
    CHECK_KEY(CURSOR_LEFT);
    CHECK_KEY(CURSOR_RIGHT);
    CHECK_KEY(CURSOR_UP);
    CHECK_KEY(CURSOR_DOWN);
    CHECK_KEY(CAPS_LOCK);
    CHECK_KEY(SCROLL_LOCK);
    CHECK_KEY(PAUSE);
    CHECK_KEY(PRINT_SCREEN);
    CHECK_KEY(NUM_LOCK);
    CHECK_KEY(LEFT_SHIFT);
    CHECK_KEY(LEFT_CONTROL);
    CHECK_KEY(LEFT_ALT);
    CHECK_KEY(RIGHT_SHIFT);
    CHECK_KEY(RIGHT_CONTROL);
    CHECK_KEY(RIGHT_ALT);
    CHECK_KEY(MENU);
    CHECK_KEY(KP_0);
    CHECK_KEY(KP_1);
    CHECK_KEY(KP_2);
    CHECK_KEY(KP_3);
    CHECK_KEY(KP_4);
    CHECK_KEY(KP_5);
    CHECK_KEY(KP_6);
    CHECK_KEY(KP_7);
    CHECK_KEY(KP_8);
    CHECK_KEY(KP_9);
    CHECK_KEY(KP_ADD);
    CHECK_KEY(KP_SUBTRACT);
    CHECK_KEY(KP_MULTIPLY);
    CHECK_KEY(KP_DIVIDE);
    CHECK_KEY(KP_DECIMAL);
    CHECK_KEY(KP_ENTER);
}

TEST(key, print_invalid)
{
    int       value    = 12345;
    mge::key *valueptr = reinterpret_cast<mge::key *>(&value);
    mge::test_stream_output(*valueptr, "INVALID(12345)");
}
