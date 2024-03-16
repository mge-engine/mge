// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/texture_type.hpp"

namespace mge {

    class MGEGRAPHICS_EXPORT texture : public context_object
    {
    protected:
        texture(render_context&, texture_type type);

    public:
        virtual ~texture() = default;

        texture_type type() const;

    private:
        texture_type m_type;
    };

} // namespace mge