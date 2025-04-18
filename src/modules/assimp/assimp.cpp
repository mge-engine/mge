// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_loader.hpp"
#include "mge/asset/asset_type.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/memory.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/image.hpp"
#include "mge/graphics/memory_image.hpp"

namespace mge {
    class assimp_loader : public asset_loader
    {
    public:
        assimp_loader() = default;
        ~assimp_loader() = default;

        std::any load(const mge::asset& a) override;

        std::span<mge::asset_type> handled_types() const override;
    };

    std::any assimp_loader::load(const mge::asset& a)
    {
        return nullptr;
    }

    std::span<mge::asset_type> assimp_loader::handled_types() const
    {
        using namespace mge::literals;
        static asset_type supported[] = {"model/obj"_at};
        return supported;
    }

    MGE_REGISTER_IMPLEMENTATION(assimp_loader, mge::asset_loader, obj);
} // namespace mge