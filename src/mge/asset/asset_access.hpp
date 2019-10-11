#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/input_stream.hpp"
namespace mge {
    MGE_DECLARE_REF(asset_access);
    
    class asset_access : public noncopyable
    {
    public:
        asset_access();
        virtual ~asset_access();
        virtual size_t size() const = 0;
        virtual input_stream_ref data() const = 0;
    };
}