#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/asset/untyped_asset.hpp"
namespace mge {

    template <typename T>
    class asset : public untyped_asset
    {
    public:
        asset(const char *path)
            :untyped_asset(path)
        {}

        asset(const std::string& path)
            :untyped_asset(path)
        {}

        asset(const untyped_asset& a)
            :untyped_asset(a.path())
        {}

        virtual ~asset() = default;
    };
}