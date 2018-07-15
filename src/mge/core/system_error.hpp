// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
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
        system_error();
        system_error(const system_error& e);
        system_error(system_error&& e);
        ~system_error();

        system_error& operator=(const system_error& e);


        static void clear();
    };

    /**
     * Helper tag structure for system error exception info.
     */
    struct tag_sysrc {};

    /**
     * @brief Exception info for system error.
     */
    typedef error_info<tag_sysrc, uint32_t> sysrc_desc;
}
