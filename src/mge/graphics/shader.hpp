#pragma once
#include "mge/core/buffer.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/shader_type.hpp"

namespace mge {
    /**
     * @brief A shader is a piece of code executed in the graphics pipeline.
     */
    class MGEGRAPHICS_EXPORT shader : public context_object
    {
    protected:
        shader(render_context& context, shader_type type);

    public:
        virtual ~shader() = default;

    protected:
        shader_type m_type;
    };
} // namespace mge