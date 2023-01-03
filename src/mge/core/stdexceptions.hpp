// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
namespace mge {

#define MGE_DECLARE_EXCEPTION_CLASS(COMPONENT, CLASS_NAME)                     \
    class COMPONENT##_EXPORT CLASS_NAME : public ::mge::exception              \
    {                                                                          \
    public:                                                                    \
        CLASS_NAME();                                                          \
        CLASS_NAME(const CLASS_NAME& e);                                       \
        CLASS_NAME(CLASS_NAME&& e);                                            \
        virtual ~CLASS_NAME() = default;                                       \
                                                                               \
        CLASS_NAME& operator=(const CLASS_NAME& e);                            \
                                                                               \
        template <typename Info> CLASS_NAME& set_info(const Info& info)        \
        {                                                                      \
            mge::exception::set_info(info);                                    \
            return *this;                                                      \
        }                                                                      \
                                                                               \
        template <typename T> CLASS_NAME& operator<<(const T& value)           \
        {                                                                      \
            mge::exception::operator<<(value);                                 \
            return *this;                                                      \
        }                                                                      \
    }

#define MGECORE_DECLARE_EXCEPTION_CLASS(CLASS_NAME)                            \
    MGE_DECLARE_EXCEPTION_CLASS(MGECORE, CLASS_NAME)

    /** @brief Thrown if the program detects an illegal state */
    MGECORE_DECLARE_EXCEPTION_CLASS(illegal_state);
    /** @brief Thrown an invalid argument is passed to a function. */
    MGECORE_DECLARE_EXCEPTION_CLASS(illegal_argument);
    /** @brief Thrown if a container is accessed outside its defined range. */
    MGECORE_DECLARE_EXCEPTION_CLASS(out_of_range);
    /** @brief Thrown if an element is duplicated in a data structure that is
     * required to be unique. */
    MGECORE_DECLARE_EXCEPTION_CLASS(duplicate_element);
    /** @brief Thrown if a type cast fails. */
    MGECORE_DECLARE_EXCEPTION_CLASS(bad_cast);
    /** @brief Thrown if an expected element is not found in a container. */
    MGECORE_DECLARE_EXCEPTION_CLASS(no_such_element);
    /** @brief Thrown if an error is detected at runtime. */
    MGECORE_DECLARE_EXCEPTION_CLASS(runtime_exception);
    /** @brief Thrown if a functionality is not (yet) implemented. */
    MGECORE_DECLARE_EXCEPTION_CLASS(not_yet_implemented);
    /** @brief Thrown if a null pointer is detected but not allowed. */
    MGECORE_DECLARE_EXCEPTION_CLASS(null_pointer);
    /** @brief Thrown if an out of memory situation is detected. */
    MGECORE_DECLARE_EXCEPTION_CLASS(out_of_memory);
    /** @brief Thrown if a number overflow is detected. */
    MGECORE_DECLARE_EXCEPTION_CLASS(numeric_overflow);

/**
 * @brief Helper macro to throw if argument is null.
 * @param N argument name
 */
#define MGE_THROW_ARGUMENT_NOT_NULL(N)                                         \
    MGE_THROW(mge::illegal_argument)                                           \
        << "Argument '" << #N << "' must not be null"

/**
 * @brief Throw not implemented error.
 */
#define MGE_THROW_NOT_IMPLEMENTED MGE_THROW(mge::not_yet_implemented)

/**
 * @brief Helper macro to define an exception class.
 */
#define MGE_DEFINE_EXCEPTION_CLASS(clazz)                                      \
    clazz::clazz() {}                                                          \
                                                                               \
    clazz::clazz(const clazz& c)                                               \
        : mge::exception(c)                                                    \
    {}                                                                         \
                                                                               \
    clazz::clazz(clazz&& c)                                                    \
        : mge::exception(std::move(c))                                         \
    {}                                                                         \
                                                                               \
    clazz& clazz::operator=(const clazz& c)                                    \
    {                                                                          \
        mge::exception::operator=(c);                                          \
        return *this;                                                          \
    }

} // namespace mge