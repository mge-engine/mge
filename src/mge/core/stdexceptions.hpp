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

        template <typename Info>
        illegal_state& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

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

        template <typename Info>
        illegal_argument& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

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

        template <typename Info>
        out_of_range& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

    };

    /**
     * @brief Duplicate element exception.
     *
     * Duplicate element exception signals that an
     * duplicate value was supplied where a duplicate is not allowed.
     *
     */
    class MGE_CORE_EXPORT duplicate_element : public ::mge::exception
    {
    public:
        duplicate_element();
        duplicate_element(const duplicate_element& e);
        duplicate_element(duplicate_element&& e);
        ~duplicate_element();

        duplicate_element& operator=(const duplicate_element& e);

        template <typename Info>
        duplicate_element& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

    };


    /**
     * @brief Bad cast exception.
     *
     * Thrown if a cast was attempted that fails the runtime check,
     * i.e. the source value cannot be represented as the target
     * type.
     */
    class MGE_CORE_EXPORT bad_cast : public ::mge::exception
    {
    public:
        bad_cast();
        bad_cast(const bad_cast& e);
        bad_cast(bad_cast&& e);
        ~bad_cast();

        bad_cast& operator=(const bad_cast& e);

        template <typename Info>
        bad_cast& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

    };


    /**
     * @brief No such element exception.
     *
     * Thrown if an element is looked up but not found.
     */
    class MGE_CORE_EXPORT no_such_element : public ::mge::exception
    {
    public:
        no_such_element();
        no_such_element(const no_such_element& e);
        no_such_element(no_such_element&& e);
        ~no_such_element();

        no_such_element& operator=(const no_such_element& e);

        template <typename Info>
        no_such_element& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }
    };


    /**
     * @brief Generic exception thrown if an error is detected.
     *
     * Thrown if a condition is detected at runtime that makes
     * it impossible to recover or continue.
     */
    class MGE_CORE_EXPORT runtime_exception : public ::mge::exception
    {
    public:
        runtime_exception();
        runtime_exception(const runtime_exception& e);
        runtime_exception(runtime_exception&& e);
        ~runtime_exception();

        runtime_exception& operator=(const runtime_exception& e);

        template <typename Info>
        runtime_exception& operator <<(const Info& info)
        {
            mge::exception::operator << (info);
            return *this;
        }

    };

}

#define MGE_DEFINE_EXCEPTION(clazz)                        \
    clazz::clazz()                                         \
    {}                                                     \
                                                           \
    clazz::clazz(const clazz& c)                           \
        :mge::exception(c)                                 \
    {}                                                     \
                                                           \
    clazz::clazz(clazz&& c)                                \
        :mge::exception(std::move(c))                      \
    {}                                                     \
                                                           \
    clazz&                                                 \
    clazz::operator = (const clazz& c)                     \
    {                                                      \
        mge::exception::operator =(c);                     \
        return *this;                                      \
    }                                                      \
                                                           \
    clazz::~clazz()                                        \
    {}
