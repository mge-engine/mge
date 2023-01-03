// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
namespace mge {

    /**
     * @brief Object that can't be copied.
     */
    class noncopyable
    {
    public:
        noncopyable() = default;
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
} // namespace mge