// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/nth_type.hpp"
#include <cstddef>
namespace mge {
    namespace internal {
        template <typename Signature> struct function_return_type;

        template <typename R, typename... Args>
        struct function_return_type<R(Args...)>
        {
            using type = R;
        };

        template <typename Signature> struct function_argument;

        template <typename R, typename... Args>
        struct function_argument<R(Args...)>
        {
            template <size_t N> using type = nth_type<N, Args...>;
        };

        template <typename Signature> struct function_signature
        {
            using return_type = typename function_return_type<Signature>::type;
        };
    } // namespace internal
} // namespace mge

#define MGE_INTERNAL_COUNT_ARGS_HELPER(x,                                      \
                                       _1,                                     \
                                       _2,                                     \
                                       _3,                                     \
                                       _4,                                     \
                                       _5,                                     \
                                       _6,                                     \
                                       _7,                                     \
                                       _8,                                     \
                                       _9,                                     \
                                       _10,                                    \
                                       _11,                                    \
                                       _12,                                    \
                                       _13,                                    \
                                       _14,                                    \
                                       _15,                                    \
                                       _16,                                    \
                                       _17,                                    \
                                       _18,                                    \
                                       _19,                                    \
                                       _20,                                    \
                                       ...)                                    \
    _20

#define MGE_INTERNAL_COUNT_ARGS(...)                                           \
    MGE_INTERNAL_COUNT_ARGS_HELPER(,                                           \
                                   ##__VA_ARGS__,                              \
                                   19,                                         \
                                   18,                                         \
                                   17,                                         \
                                   16,                                         \
                                   15,                                         \
                                   14,                                         \
                                   13,                                         \
                                   12,                                         \
                                   11,                                         \
                                   10,                                         \
                                   9,                                          \
                                   8,                                          \
                                   7,                                          \
                                   6,                                          \
                                   5,                                          \
                                   4,                                          \
                                   3,                                          \
                                   2,                                          \
                                   1,                                          \
                                   0)

#define MGE_INTERNAL_ASSERT_PARENTHESIS(...)                                   \
    static_assert(MGE_INTERNAL_COUNT_ARGS(__VA_ARGS__) == 1,                   \
                  "Arguments must be enclosed in parenthesis")

#define MGE_INTERNAL_PARAMETER_SIGNATURE(RETURN_TYPE, ARGS)                    \
    MGE_INTERNAL_PARAMETER_SIGNATURE_1(MGE_INTERNAL_COUNT_ARGS ARGS,           \
                                       RETURN_TYPE,                            \
                                       ARGS)

#define MGE_INTERNAL_CONCAT(A, B) A##B

#define MGE_INTERNAL_PARAMETER_SIGNATURE_1(N, RETURN_TYPE, ARGS)               \
    MGE_INTERNAL_CONCAT(MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_, N)           \
    (RETURN_TYPE, ARGS)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_0(RETURN_TYPE, ARGS) ()

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_1(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_2(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_3(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_4(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_5(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_6(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_7(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_8(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_9(RETURN_TYPE, ARGS)           \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_10(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9)
#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_11(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_12(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_13(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_14(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<13>              \
         __mge_script_arg13)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_15(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<13>              \
         __mge_script_arg13,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<14>              \
         __mge_script_arg14)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_16(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<13>              \
         __mge_script_arg13,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<14>              \
         __mge_script_arg14,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<15>              \
         __mge_script_arg15)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_17(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<13>              \
         __mge_script_arg13,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<14>              \
         __mge_script_arg14,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<15>              \
         __mge_script_arg15,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<16>              \
         __mge_script_arg16)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_18(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<13>              \
         __mge_script_arg13,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<14>              \
         __mge_script_arg14,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<15>              \
         __mge_script_arg15,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<16>              \
         __mge_script_arg16,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<17>              \
         __mge_script_arg17)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_19(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<13>              \
         __mge_script_arg13,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<14>              \
         __mge_script_arg14,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<15>              \
         __mge_script_arg15,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<16>              \
         __mge_script_arg16,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<17>              \
         __mge_script_arg17,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<18>              \
         __mge_script_arg18)

#define MGE_INTERNAL_PARAMETER_SIGNATURE_EXPAND_20(RETURN_TYPE, ARGS)          \
    (mge::internal::function_argument<RETURN_TYPE ARGS>::type<0>               \
         __mge_script_arg0,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<1>               \
         __mge_script_arg1,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<2>               \
         __mge_script_arg2,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<3>               \
         __mge_script_arg3,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<4>               \
         __mge_script_arg4,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<5>               \
         __mge_script_arg5,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<6>               \
         __mge_script_arg6,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<7>               \
         __mge_script_arg7,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<8>               \
         __mge_script_arg8,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<9>               \
         __mge_script_arg9,                                                    \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<10>              \
         __mge_script_arg10,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<11>              \
         __mge_script_arg11,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<12>              \
         __mge_script_arg12,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<13>              \
         __mge_script_arg13,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<14>              \
         __mge_script_arg14,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<15>              \
         __mge_script_arg15,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<16>              \
         __mge_script_arg16,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<17>              \
         __mge_script_arg17,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<18>              \
         __mge_script_arg18,                                                   \
     mge::internal::function_argument<RETURN_TYPE ARGS>::type<19>              \
         __mge_script_arg19)

#define MGE_IMPLEMENT_METHOD(RETURN_TYPE, METHOD_NAME, ARGS, QUALIFIERS)       \
    mge::internal::function_signature<RETURN_TYPE ARGS>::return_type           \
        METHOD_NAME                                                            \
        MGE_INTERNAL_PARAMETER_SIGNATURE(RETURN_TYPE, ARGS) QUALIFIERS         \
    {                                                                          \
        MGE_INTERNAL_ASSERT_PARENTHESIS(ARGS);                                 \
                                                                               \
        if (!m_context) {                                                      \
            MGE_THROW(mge::illegal_state)                                      \
                << "No context set for call to '" << #METHOD_NAME << "'";      \
        }                                                                      \
                                                                               \
        if constexpr (std::is_same_v<RETURN_TYPE, void>) {                     \
            /* void type */                                                    \
            m_context->invoke_method<void>(#METHOD_NAME);                      \
            return;                                                            \
        } else {                                                               \
            /* non void */                                                     \
            return m_context->invoke_method<RETURN_TYPE>(#METHOD_NAME);        \
        }                                                                      \
    }
