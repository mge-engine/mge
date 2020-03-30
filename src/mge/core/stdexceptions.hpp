// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/exception.hpp"
#include "mge/core/types.hpp"

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
        illegal_state(const illegal_state &e);
        illegal_state(illegal_state &&e);
        ~illegal_state();

        illegal_state &operator=(const illegal_state &e);

        template <typename Info> illegal_state &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> illegal_state &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
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
        illegal_argument(const illegal_argument &e);
        illegal_argument(illegal_argument &&e);
        ~illegal_argument();

        illegal_argument &operator=(const illegal_argument &e);

        template <typename Info> illegal_argument &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> illegal_argument &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
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
        out_of_range(const out_of_range &e);
        out_of_range(out_of_range &&e);
        ~out_of_range();

        out_of_range &operator=(const out_of_range &e);

        template <typename Info> out_of_range &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> out_of_range &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
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
        duplicate_element(const duplicate_element &e);
        duplicate_element(duplicate_element &&e);
        ~duplicate_element();

        duplicate_element &operator=(const duplicate_element &e);

        template <typename Info> duplicate_element &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> duplicate_element &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
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
        bad_cast(const bad_cast &e);
        bad_cast(bad_cast &&e);
        ~bad_cast();

        bad_cast &operator=(const bad_cast &e);

        template <typename Info> bad_cast &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> bad_cast &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
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
        no_such_element(const no_such_element &e);
        no_such_element(no_such_element &&e);
        ~no_such_element();

        no_such_element &operator=(const no_such_element &e);

        template <typename Info> no_such_element &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> no_such_element &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
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
        runtime_exception(const runtime_exception &e);
        runtime_exception(runtime_exception &&e);
        ~runtime_exception();

        runtime_exception &operator=(const runtime_exception &e);

        template <typename Info> runtime_exception &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> runtime_exception &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };

    /**
     * @brief Not implemented exception.
     *
     * Raised if a code path is hit that has not been implemented.
     */
    class MGE_CORE_EXPORT not_yet_implemented : public ::mge::exception
    {
    public:
        not_yet_implemented();
        not_yet_implemented(const not_yet_implemented &e);
        not_yet_implemented(not_yet_implemented &&e);
        ~not_yet_implemented();

        not_yet_implemented &operator=(const not_yet_implemented &e);

        template <typename Info> not_yet_implemented &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> not_yet_implemented &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };

    /**
     * @brief Null pointer exception.
     *
     * Raised if an unexpected null pointer is found.
     */
    class MGE_CORE_EXPORT null_pointer : public ::mge::exception
    {
    public:
        null_pointer();
        null_pointer(const null_pointer &e);
        null_pointer(null_pointer &&e);
        ~null_pointer();

        null_pointer &operator=(const null_pointer &e);

        template <typename Info> null_pointer &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> null_pointer &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };

} // namespace mge

#define MGE_THROW_ARGUMENT_NOT_NULL(N)                                         \
    MGE_THROW(mge::illegal_argument)                                           \
        << "Argument '" << #N << "' must not be null"

#define MGE_THROW_NOT_IMPLEMENTED MGE_THROW(mge::not_yet_implemented)

#define MGE_DEFINE_EXCEPTION(clazz)                                            \
    clazz::clazz() {}                                                          \
                                                                               \
    clazz::clazz(const clazz &c) : mge::exception(c) {}                        \
                                                                               \
    clazz::clazz(clazz &&c) : mge::exception(std::move(c)) {}                  \
                                                                               \
    clazz &clazz::operator=(const clazz &c)                                    \
    {                                                                          \
        mge::exception::operator=(c);                                          \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    clazz::~clazz() {}
