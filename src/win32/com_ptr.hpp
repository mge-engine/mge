// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include <memory>
#include <windows.h>
#include <wrl/client.h>

namespace mge {

    template <typename T> using com_ptr = ::Microsoft::WRL::ComPtr<T>;

}
