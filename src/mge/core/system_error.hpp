// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif

namespace mge {

    /**
     * @brief A system error.
     *
     * Constructing a system error retrieves the last system
     * error.
     */
    class MGECORE_EXPORT system_error : public exception
    {
    public:
#ifdef MGE_OS_WINDOWS
        using error_code_type = DWORD;
#elif defined(MGE_OS_LINUX)
        using error_code_type = int;
#elif defined(MGE_OS_MACOSX)
        using error_code_type = int;
#else
#    error Missing port
#endif

        /**
         * @brief System error code attached to exception.
         */
        struct error_code : public exception::tag<error_code, error_code_type>
        {
            /**
             * @brief Capture error code.
             * @param value_ error code
             */
            error_code(error_code_type value_) noexcept
                : m_value(value_)
            {}

            error_code_type value() const noexcept
            {
                return m_value;
            }

            error_code_type m_value;
        };

        /**
         * @brief Construct empty error.
         */
        system_error();

        /**
         * @brief Construct system error from system dependent error code type.
         *
         * @param code system error code
         */
        system_error(error_code_type code);

        /**
         * @brief Copy constructor.
         *
         * @param e copied error
         */
        system_error(const system_error& e);

        /**
         * @brief Move constructor.
         *
         * @param e moved error
         */
        system_error(system_error&& e);

        ~system_error();

        system_error& operator=(const system_error& e);

        template <typename Info> system_error& set_info(const Info& info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> system_error& operator<<(const T& value)
        {
            mge::exception::operator<<(value);
            return *this;
        }

        static void check_error(const char* file,
                                int         line,
                                const char* signature,
                                const char* function);

    private:
        void set_error_code(error_code_type ec);
    };

#define MGE_CHECK_SYSTEM_ERROR(function)                                       \
    ::mge::system_error::check_error(__FILE__,                                 \
                                     __LINE__,                                 \
                                     MGE_FUNCTION_SIGNATURE,                   \
                                     #function)

} // namespace mge

template <>
struct fmt::formatter<mge::system_error> : public fmt::formatter<mge::exception>
{};
