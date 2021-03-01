// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
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
            error_code(error_code_type value_) noexcept : m_value(value_) {}

            error_code_type value() const noexcept { return m_value; }

            error_code_type m_value;
        };

        system_error();
        system_error(error_code_type code);
        system_error(const system_error &e);
        system_error(system_error &&e);

        ~system_error();

        system_error &operator=(const system_error &e);

        template <typename Info> system_error &set_info(const Info &info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> system_error &operator<<(const T &value)
        {
            mge::exception::operator<<(value);
            return *this;
        }
    };

} // namespace mge