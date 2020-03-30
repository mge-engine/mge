// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
#include <boost/system/system_error.hpp>
namespace mge {

    /**
     * @brief System error.
     * A system error is thrown if a system call fails, as usually
     * indicated by @c errno or @c GetLastError() returning an error
     * code.
     */
    class MGE_CORE_EXPORT system_error : public exception
    {
    public:
        struct MGE_CORE_EXPORT error
            : exception::tag<error, std::tuple<uint32_t, std::string>>
        {
            error();
            error(const boost::system::error_code &ec);

            auto value() const noexcept
            {
                return std::make_tuple(m_error_value, m_error_message);
            }

            uint32_t    m_error_value;
            std::string m_error_message;
        };

        system_error();
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

        /**
         * Clears the system error stored for the current thread.
         */
        static void clear();

        const char *what() const override;

    private:
        mutable std::string m_message;
    };

/**
 * Throws the current system error.
 */
#define MGE_THROW_CURRENT_SYSTEM_ERROR                                         \
    MGE_THROW(mge::system_error).set_info(mge::system_error::error())

/**
 * Throws a system error that has been reported before.
 * @param error_code error code retrieved already before
 */
#define MGE_THROW_SYSTEM_ERROR(error_code)                                     \
    MGE_THROW(mge::system_error).set_info(mge::system_error::error(error_code))

#define MGE_THROW_SYSCALL_FAILED(F)                                            \
    MGE_THROW_CURRENT_SYSTEM_ERROR.set_info(MGE_CALLED_FUNCTION(F))

} // namespace mge
