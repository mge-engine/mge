// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

// see https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
// for elaboration on how this works

#define MGE_NARGS_SELECT(_0,                                                   \
                         _1,                                                   \
                         _2,                                                   \
                         _3,                                                   \
                         _4,                                                   \
                         _5,                                                   \
                         _6,                                                   \
                         _7,                                                   \
                         _8,                                                   \
                         _9,                                                   \
                         _10,                                                  \
                         _11,                                                  \
                         _12,                                                  \
                         _13,                                                  \
                         _14,                                                  \
                         _15,                                                  \
                         _16,                                                  \
                         _17,                                                  \
                         _18,                                                  \
                         _19,                                                  \
                         _20,                                                  \
                         _21,                                                  \
                         _22,                                                  \
                         _23,                                                  \
                         _24,                                                  \
                         _25,                                                  \
                         _26,                                                  \
                         _27,                                                  \
                         _28,                                                  \
                         _29,                                                  \
                         _30,                                                  \
                         _31,                                                  \
                         N,                                                    \
                         ...)                                                  \
    N

#define MGE_HAS_COMMA(...)                                                     \
    MGE_NARGS_SELECT(__VA_ARGS__,                                              \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     1,                                                        \
                     0)

#define MGE_NARGS(...)                                                         \
    MGE_NARGS_SELECT(__VA_ARGS__,                                              \
                     32,                                                       \
                     31,                                                       \
                     30,                                                       \
                     29,                                                       \
                     28,                                                       \
                     27,                                                       \
                     26,                                                       \
                     25,                                                       \
                     24,                                                       \
                     23,                                                       \
                     22,                                                       \
                     21,                                                       \
                     20,                                                       \
                     19,                                                       \
                     18,                                                       \
                     17,                                                       \
                     16,                                                       \
                     15,                                                       \
                     14,                                                       \
                     13,                                                       \
                     12,                                                       \
                     11,                                                       \
                     10,                                                       \
                     9,                                                        \
                     8,                                                        \
                     7,                                                        \
                     6,                                                        \
                     5,                                                        \
                     4,                                                        \
                     3,                                                        \
                     2,                                                        \
                     1,                                                        \
                     0)

#define MGE_TRIGGER_PARENTHESIS(...) ,

#define MGE_IS_EMPTY(...)                                                      \
    MGE_IS_EMPTY_2(                                                            \
        MGE_HAS_COMMA(__VA_ARGS__),                                            \
        MGE_HAS_COMMA(MGE_TRIGGER_PARENTHESIS __VA_ARGS__),                    \
        MGE_HAS_COMMA(__VA_ARGS__(/*empty*/)),                                 \
        MGE_HAS_COMMA(MGE_TRIGGER_PARENTHESIS __VA_ARGS__(/*empty*/)))

#define MGE_IS_EMPTY_CASE_0001 ,
#define MGE_IS_EMPTY_PASTE5(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4

#define MGE_IS_EMPTY_2(_0, _1, _2, _3)                                         \
    MGE_HAS_COMMA(MGE_IS_EMPTY_PASTE5(MGE_IS_EMPTY_CASE_, _0, _1, _2, _3))