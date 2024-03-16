// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_fwd.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/component.hpp"
#include <any>
#include <functional>
#include <span>

namespace mge {

    class MGEASSET_EXPORT asset_loader : public component<asset_loader>
    {
    public:
        asset_loader() = default;
        virtual ~asset_loader() = default;
        virtual std::span<asset_type> handled_types() const = 0;
    };

} // namespace mge