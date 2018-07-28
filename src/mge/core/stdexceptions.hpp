// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/exception.hpp"

namespace mge {

    /**
     * @brief Illegal state exception.
     *
     * Illegal state exception signals a method has been invoked
     * at an illegal or inappropriate time, and the requested
     * operation cannot be performed.
     */
    class MGE_CORE_EXPORT illegal_state : public ::mge::exception
    {
    public:
        illegal_state();
        illegal_state(const illegal_state& e);
        illegal_state(illegal_state&& e);
        ~illegal_state();

        illegal_state& operator=(const illegal_state& e);
    };


    /**
     * @brief Illegal argument exception.
     *
     * Illegal argument exception signals a method has been passed
     * an illegal argument.
     */
    class MGE_CORE_EXPORT illegal_argument : public ::mge::exception
    {
    public:
        illegal_argument();
        illegal_argument(const illegal_argument& e);
        illegal_argument(illegal_argument&& e);
        ~illegal_argument();

        illegal_argument& operator=(const illegal_argument& e);
    };


    /**
     * @brief Out of range exception.
     *
     * Out of range exception signals a failed attempt to access
     * an element out of the defined range.
     */
    class MGE_CORE_EXPORT out_of_range : public ::mge::exception
    {
    public:
        out_of_range();
        out_of_range(const out_of_range& e);
        out_of_range(out_of_range&& e);
        ~out_of_range();

        out_of_range& operator=(const out_of_range& e);
    };


}
