#pragma once
#include "mge/graphics/pipeline.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class render_context;

    class pipeline : public mge::pipeline
    {
    public:
        pipeline(render_context& context);
        virtual ~pipeline();
    };
} // namespace mge::opengl