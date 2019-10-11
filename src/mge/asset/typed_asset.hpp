#pragma once
#include "mge/asset/dllexport.hpp"
#include "mge/asset/asset.hpp"
namespace mge {

    /**
     * A typed asset can be used to load/store a certain
     * asset type.
     */
    template <typename T>
    class typed_asset : public asset 
    {
    public:
        typed_asset(const char *path)
            :asset(path)
        {}

        typed_asset(const std::string& path)
            :asset(path)
        {}

        typed_asset(const asset& a)
            :asset(a)
        {}

        virtual ~typed_asset() = default;
    };
}