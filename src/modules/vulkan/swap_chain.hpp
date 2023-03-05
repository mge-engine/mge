// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/swap_chain.hpp"

namespace mge::vulkan {
    class render_context;

    class swap_chain : public mge::swap_chain
    {
    public:
        swap_chain(render_context& context);
        virtual ~swap_chain();
        void present() override;
    };

} // namespace mge::vulkan