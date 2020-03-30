// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/texture_2d.hpp"
#include "win32/com_unique_ptr.hpp"
#include <variant>

namespace dx11 {
    class texture_2d : public mge::texture_2d
    {
    public:
        texture_2d(mge::render_context &context, const mge::image_ref &image);
        texture_2d(mge::render_context &context);
        virtual ~texture_2d();

    private:
        void create_texture(const mge::image_ref &image);
        using texture_t = mge::com_unique_ptr<ID3D11Texture2D>;
        texture_t m_texture;
    };
} // namespace dx11
