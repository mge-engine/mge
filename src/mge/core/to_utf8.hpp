// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/config.hpp"

#include <codecvt>
#include <string>

namespace mge {

#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 4996)
#endif

    inline std::string to_utf8(const wchar_t* str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(str);
    }

    inline std::string to_utf8(const std::wstring& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(str);
    }

#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 4996)
#endif

} // namespace mge