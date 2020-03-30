// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/asset/asset_type.hpp"
#include "mge/asset/dllexport.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/core/properties.hpp"
#include "mge/core/types.hpp"

namespace mge {
    MGE_DECLARE_REF(asset_access);

    class MGE_ASSET_EXPORT asset_access : public noncopyable
    {
    public:
        asset_access();
        virtual ~asset_access();
        virtual size_t           size() const           = 0;
        virtual input_stream_ref data() const           = 0;
        virtual asset_type       type() const           = 0;
        virtual bool             has_properties() const = 0;
        virtual properties_ref   properties() const     = 0;
    };
} // namespace mge
