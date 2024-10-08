// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class error : public mge::exception
    {
    public:
        error();
        error(const error& e);
        error(error&& e);
        ~error() = default;

        error& operator=(const error& e);

        template <typename Info> error& set_info(const Info& info)
        {
            mge::exception::set_info(info);
            return *this;
        }

        template <typename T> error& operator<<(const T& value)
        {
            mge::exception::operator<<(value);
            return *this;
        }

        static VkResult check_vkresult(VkResult    rc,
                                       const char* file,
                                       int         line,
                                       const char* function);

    private:
        error& set_info_from_vkresult(VkResult    rc,
                                      const char* file,
                                      int         line,
                                      const char* function);
    };
} // namespace mge::vulkan

#define CHECK_VKRESULT(rc, function)                                           \
    ::mge::vulkan::error::check_vkresult(rc, __FILE__, __LINE__, #function)

#define CHECK_VK_CALL(call) CHECK_VK_CALL2(call, __FILE__, __LINE__)

#define CHECK_VK_CALL2(call, file, line)                                       \
    do {                                                                       \
        auto rc = call;                                                        \
        ::mge::vulkan::error::check_vkresult(rc, file, line, #call);           \
    } while (false)