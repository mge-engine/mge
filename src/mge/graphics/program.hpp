// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "mge/core/property_object.hpp"
#include "mge/core/small_vector.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/shader_handle.hpp"
#include "mge/graphics/uniform_data_type.hpp"

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace mge {

    class uniform_block;

    /**
     * @brief A program represents the stages in the drawing process.
     *
     * When applied to a render command list, a program executes several
     * stages:
     * - input assembly, where bound buffers are processed and vertices are
     *   generated depending on the drawing instructions
     * - vertex processing, where vertex, tessellation and geometry shaders
     *   are executed, resulting in transformed output vertices
     * - clipping, where a subset of vertices is selected for further processing
     * - rasterization, where frame buffer addresses (fragments) are computed
     * - fragment processing, where fragment (pixel) shaders are executed for
     *   each fragment
     * - frame buffer operations, where the computed fragment is applied to the
     *   frame buffer, with possible depth buffering, blending, masking,
     * stenciling or other similar operations
     *
     * programs are either implemented with corresponding objects in the render
     * system, or handled in software by executing a number of calls that
     * implement the mentioned stages.
     */
    class MGEGRAPHICS_EXPORT program : public context_object
    {
    protected:
        program(render_context& context);

    public:
        /// Attribute description
        struct attribute
        {
            std::string name; //!< attribute name
            data_type   type; //!< attribute type
            uint8_t     size; //!< attribute size (array size)
        };

        // list of attributes
        using attribute_list = small_vector<attribute, 5>;

        /// Uniform description
        struct uniform
        {
            std::string       name;       //!< uniform name
            uniform_data_type type;       //!< uniform type
            uint32_t          array_size; //!< uniform array size
            uint32_t          location;   //!< uniform location
        };

        using uniform_list = small_vector<uniform, 5>;

        struct uniform_buffer
        {
            std::string  name;
            uniform_list uniforms;
        };

        using uniform_buffer_list = small_vector<uniform_buffer, 3>;

        virtual ~program();

        /**
         * Set a shader object. The shader object must be a shader designated
         * for the graphics program (not a compute shader).
         *
         * @param shader shader object that is set
         */
        void set_shader(shader_handle shader);

        /**
         * Links the program. A program must be linked after setting all
         * shader programs.
         */
        void link();

        /**
         * Return whether this program needs the link step.
         *
         * @return @c true if program needs to be linked
         */
        bool needs_link() const noexcept
        {
            return m_needs_link;
        }

        /**
         * Get meta data of the attributes.
         *
         * @return attributes
         */
        const attribute_list& attributes() const;

        /**
         * Get meta data of uniforms.
         *
         * @return uniforms
         */
        const uniform_list& uniforms() const;

        /**
         * Get uniform buffer meta data.
         * @return uniform buffers
         */
        const uniform_buffer_list& uniform_buffers() const;

        /**
         * @brief Create a uniform block instance from one of this program's
         * uniform buffers.
         *
         * @param block_name name of the uniform buffer in the shader
         * @return a new uniform_block with std140 layout
         */
        uniform_block create_uniform_block(const std::string& block_name) const;

    protected:
        bool                m_needs_link;
        attribute_list      m_attributes;
        uniform_list        m_uniforms;
        uniform_buffer_list m_uniform_buffers;

    private:
        void assert_linked() const;

        virtual void on_link() = 0;
        virtual void on_set_shader(shader* shader) = 0;
    };

    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&             os,
                                                const program::attribute& a);
    MGEGRAPHICS_EXPORT std::ostream& operator<<(std::ostream&           os,
                                                const program::uniform& u);
    MGEGRAPHICS_EXPORT               std::ostream&
    operator<<(std::ostream& os, const program::uniform_buffer& ub);
} // namespace mge

template <> struct fmt::formatter<mge::program::attribute>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mge::program::attribute& attr, FormatContext& ctx) const
        -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(),
                              "attribute{{ name: '{}', type: {}, size: {} }}",
                              attr.name,
                              attr.type,
                              attr.size);
    }
};

template <> struct fmt::formatter<mge::program::uniform>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mge::program::uniform& uniform, FormatContext& ctx) const
        -> decltype(ctx.out())
    {
        return fmt::format_to(
            ctx.out(),
            "uniform{{ name: '{}', type: {}, array_size: {}, location: {} }}",
            uniform.name,
            uniform.type,
            uniform.array_size,
            uniform.location);
    }
};

template <> struct fmt::formatter<mge::program::uniform_buffer>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mge::program::uniform_buffer& uniform_buffer,
                FormatContext&                      ctx) const
    {
        return fmt::format_to(ctx.out(),
                              "uniform_buffer{{ name: '{}', size: {} }}",
                              uniform_buffer.name,
                              uniform_buffer.uniforms.size());
    }
};
