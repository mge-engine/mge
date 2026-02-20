// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/ui/ui.hpp"
#include "mge/core/buffer.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/attribute_semantic.hpp"
#include "mge/graphics/blend_factor.hpp"
#include "mge/graphics/blend_operation.hpp"
#include "mge/graphics/command_buffer.hpp"
#include "mge/graphics/cull_mode.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/graphics/image_format.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/pass.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/shader_format.hpp"
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/test.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/graphics/vertex_format.hpp"
#include "mge/input/input_handler.hpp"
#include "mge/input/key.hpp"
#include "mge/input/key_action.hpp"
#include "mge/input/modifier.hpp"
#include "mge/input/mouse_action.hpp"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_COMMAND_USERDATA

#ifdef _MSC_VER
#    pragma warning(push)
#    pragma warning(disable : 4701) // potentially uninitialized variable
#    pragma warning(disable : 4706) // assignment within conditional expression
#endif

#include <nuklear.h>

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

namespace mge {
    MGE_USE_TRACE(UI);

    static nk_flags to_nk_flags(const ui_window_flags& flags)
    {
        const auto& bits = static_cast<
            const std::bitset<static_cast<size_t>(ui_window_flag::MAX)>&>(
            flags);
        return static_cast<nk_flags>(bits.to_ulong());
    }

    static nk_flags to_nk_flags(const ui_alignment& flags)
    {
        const auto& bits = static_cast<
            const std::bitset<static_cast<size_t>(ui_alignment_flag::MAX)>&>(
            flags);
        return static_cast<nk_flags>(bits.to_ulong());
    }

    static void* nk_mge_alloc(nk_handle unused, void* old, nk_size size)
    {
        (void)unused;
        return mge::realloc(old, size);
    }

    static void nk_mge_free(nk_handle unused, void* ptr)
    {
        (void)unused;
        mge::free(ptr);
    }

    class nk_allocator_instance
    {
    public:
        nk_allocator_instance()
        {
            m_allocator.userdata = nk_handle_ptr(nullptr);
            m_allocator.alloc = nk_mge_alloc;
            m_allocator.free = nk_mge_free;
        }

        nk_allocator* get()
        {
            return &m_allocator;
        }

        static mge::singleton<nk_allocator_instance> instance;

    private:
        nk_allocator m_allocator;
    };

    mge::singleton<nk_allocator_instance> nk_allocator_instance::instance;

    ui::ui(render_context& context)
        : m_render_context(&context)
        , m_context(new nk_context())
        , m_font_atlas(new nk_font_atlas())
        , m_commands(new nk_buffer())
        , m_vertices(new nk_buffer())
        , m_indices(new nk_buffer())
        , m_convert_config(new nk_convert_config())
    {
        // Setup vertex layout for UI: position(vec2) + texcoord(vec2) +
        // color(rgba8)
        m_vertex_layout.push_back(vertex_format(data_type::FLOAT, 2),
                                  attribute_semantic::POSITION);
        m_vertex_layout.push_back(vertex_format(data_type::FLOAT, 2),
                                  attribute_semantic::TEXCOORD);
        m_vertex_layout.push_back(vertex_format(data_type::UINT8, 4),
                                  attribute_semantic::COLOR);

        // Initialize nuklear buffers
        nk_buffer_init(m_commands,
                       nk_allocator_instance::instance->get(),
                       NK_BUFFER_DEFAULT_INITIAL_SIZE);
        nk_buffer_init(m_vertices,
                       nk_allocator_instance::instance->get(),
                       NK_BUFFER_DEFAULT_INITIAL_SIZE);
        nk_buffer_init(m_indices,
                       nk_allocator_instance::instance->get(),
                       NK_BUFFER_DEFAULT_INITIAL_SIZE);

        // Setup nuklear convert config
        // nk_draw_vertex: vec2(pos) + vec2(uv) + vec4(rgba8) = 20 bytes
        static const struct nk_draw_vertex_layout_element vertex_layout[] = {
            {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, 0},
            {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, 8},
            {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, 16},
            {NK_VERTEX_LAYOUT_END}};
        memset(m_convert_config, 0, sizeof(nk_convert_config));
        m_convert_config->vertex_layout = vertex_layout;
        m_convert_config->vertex_size = 20;
        m_convert_config->vertex_alignment = 4;
        m_convert_config->circle_segment_count = 22;
        m_convert_config->curve_segment_count = 22;
        m_convert_config->arc_segment_count = 22;
        m_convert_config->global_alpha = 1.0f;
        m_convert_config->shape_AA = NK_ANTI_ALIASING_ON;
        m_convert_config->line_AA = NK_ANTI_ALIASING_ON;

        // Initialize font atlas with custom allocator
        nk_font_atlas_init(m_font_atlas,
                           nk_allocator_instance::instance->get());

        // Begin font atlas baking
        nk_font_atlas_begin(m_font_atlas);

        // Add default font at 13px
        nk_font* font = nk_font_atlas_add_default(m_font_atlas, 13.0f, nullptr);

        // Bake font atlas
        int         width, height;
        const void* image = nk_font_atlas_bake(m_font_atlas,
                                               &width,
                                               &height,
                                               NK_FONT_ATLAS_RGBA32);

        // Create and upload font texture
        m_font_texture =
            m_render_context->create_texture(texture_type::TYPE_2D);
        image_format format(image_format::data_format::RGBA, data_type::UINT8);
        extent       tex_extent(static_cast<uint32_t>(width),
                          static_cast<uint32_t>(height));
        m_font_texture->set_data(format,
                                 tex_extent,
                                 image,
                                 static_cast<size_t>(width * height * 4));

        // End font atlas baking with texture handle
        m_null_texture = new nk_draw_null_texture();
        nk_font_atlas_end(m_font_atlas,
                          nk_handle_ptr(m_font_texture.get()),
                          m_null_texture);
        m_convert_config->tex_null = *m_null_texture;

        // Create shader program
        auto vertex_shader =
            m_render_context->create_shader(shader_type::VERTEX);
        auto fragment_shader =
            m_render_context->create_shader(shader_type::FRAGMENT);

        // Detect shader language from render system capabilities
        const auto& caps =
            m_render_context->render_system().system_capabilities();
        const auto& languages = caps.shader_languages();
        const auto& formats = caps.shader_formats();
        bool        use_hlsl = false;
        bool        use_spirv = false;
        for (const auto& lang : languages) {
            if (lang.name() == "hlsl") {
                use_hlsl = true;
                break;
            }
        }
        for (const auto& fmt : formats) {
            if (fmt.name() == "spirv") {
                use_spirv = true;
                break;
            }
        }

        if (use_hlsl) {
            // HLSL shaders for DirectX 11/12
            const char* vertex_shader_hlsl = R"shader(
                cbuffer UBO : register(b0) {
                    float4x4 projection;
                };

                struct VS_INPUT {
                    float2 position : POSITION;
                    float2 texcoord : TEXCOORD;
                    float4 color : COLOR;
                };

                struct VS_OUTPUT {
                    float4 position : SV_POSITION;
                    float2 texcoord : TEXCOORD;
                    float4 color : COLOR;
                };

                VS_OUTPUT main(VS_INPUT input) {
                    VS_OUTPUT output;
                    output.position = mul(projection, float4(input.position, 0.0, 1.0));
                    output.texcoord = input.texcoord;
                    output.color = input.color;
                    return output;
                }
            )shader";

            const char* fragment_shader_hlsl = R"shader(
                Texture2D tex : register(t0);
                SamplerState sam : register(s0);

                struct PS_INPUT {
                    float4 position : SV_POSITION;
                    float2 texcoord : TEXCOORD;
                    float4 color : COLOR;
                };

                float4 main(PS_INPUT input) : SV_Target {
                    return input.color * tex.Sample(sam, input.texcoord);
                }
            )shader";

            vertex_shader->compile(vertex_shader_hlsl);
            fragment_shader->compile(fragment_shader_hlsl);
        } else if (use_spirv) {
            // Vulkan shaders (GLSL 450 with Vulkan bindings)
            const char* vertex_shader_vk = R"shader(
                #version 450
                layout(location = 0) in vec2 position;
                layout(location = 1) in vec2 texcoord;
                layout(location = 2) in vec4 color;

                layout(location = 0) out vec2 frag_texcoord;
                layout(location = 1) out vec4 frag_color;

                layout(binding = 0) uniform UBO {
                    mat4 projection;
                };

                void main() {
                    frag_texcoord = texcoord;
                    frag_color = color;
                    gl_Position = projection * vec4(position, 0.0, 1.0);
                }
            )shader";

            const char* fragment_shader_vk = R"shader(
                #version 450
                layout(location = 0) in vec2 frag_texcoord;
                layout(location = 1) in vec4 frag_color;

                layout(location = 0) out vec4 out_color;

                layout(binding = 1) uniform sampler2D tex;

                void main() {
                    out_color = frag_color * texture(tex, frag_texcoord);
                }
            )shader";

            vertex_shader->compile(vertex_shader_vk);
            fragment_shader->compile(fragment_shader_vk);
        } else {
            // OpenGL shaders (GLSL 330 core)
            const char* vertex_shader_glsl = R"shader(
                #version 330 core
                layout(location = 0) in vec2 position;
                layout(location = 1) in vec2 texcoord;
                layout(location = 2) in vec4 color;

                out vec2 frag_texcoord;
                out vec4 frag_color;

                layout(std140) uniform UBO {
                    mat4 projection;
                };

                void main() {
                    frag_texcoord = texcoord;
                    frag_color = color;
                    gl_Position = projection * vec4(position, 0.0, 1.0);
                }
            )shader";

            const char* fragment_shader_glsl = R"shader(
                #version 330 core
                in vec2 frag_texcoord;
                in vec4 frag_color;

                out vec4 out_color;

                uniform sampler2D tex;

                void main() {
                    out_color = frag_color * texture(tex, frag_texcoord);
                }
            )shader";

            vertex_shader->compile(vertex_shader_glsl);
            fragment_shader->compile(fragment_shader_glsl);
        }

        m_ui_program = m_render_context->create_program();
        m_ui_program->set_shader(vertex_shader);
        m_ui_program->set_shader(fragment_shader);
        m_ui_program->link();

        // Note: uniform block created in draw() after program linking completes

        // Allocate static vertex and index buffers (64KB vertex, 32KB index)
        constexpr size_t max_vertex_buffer_size = 64 * 1024;
        constexpr size_t max_index_buffer_size = 32 * 1024;

        m_vertex_buffer =
            m_render_context->create_vertex_buffer(m_vertex_layout,
                                                   max_vertex_buffer_size,
                                                   nullptr);
        m_index_buffer =
            m_render_context->create_index_buffer(data_type::UINT16,
                                                  max_index_buffer_size,
                                                  nullptr);

        // Initialize context with allocator and font
        nk_init(m_context,
                nk_allocator_instance::instance->get(),
                &font->handle);

        start_frame();
    }

    ui::~ui()
    {
        detach();
        if (m_uniform_block) {
            delete m_uniform_block;
            m_uniform_block = nullptr;
        }
        if (m_context) {
            nk_free(m_context);
            delete m_context;
            m_context = nullptr;
        }
        if (m_font_atlas) {
            nk_font_atlas_clear(m_font_atlas);
            delete m_font_atlas;
            m_font_atlas = nullptr;
        }
        if (m_commands) {
            nk_buffer_free(m_commands);
            delete m_commands;
            m_commands = nullptr;
        }
        if (m_vertices) {
            nk_buffer_free(m_vertices);
            delete m_vertices;
            m_vertices = nullptr;
        }
        if (m_indices) {
            nk_buffer_free(m_indices);
            delete m_indices;
            m_indices = nullptr;
        }
        if (m_convert_config) {
            delete m_convert_config;
            m_convert_config = nullptr;
        }
        if (m_null_texture) {
            delete m_null_texture;
            m_null_texture = nullptr;
        }
    }

    void ui::start_frame()
    {
        nk_input_begin(m_context);
        m_in_frame = false;
    }

    void ui::begin_frame()
    {
        nk_input_end(m_context);
        m_in_frame = true;
    }

    void ui::frame()
    {
        if (!m_in_frame) {
            MGE_THROW(mge::illegal_state)
                << "Cannot end frame without beginning it first";
        }
        m_in_frame = false;
    }

    bool ui::begin_window(const char*     title,
                          float           x,
                          float           y,
                          float           width,
                          float           height,
                          ui_window_flags flags)
    {
        return nk_begin(m_context,
                        title,
                        nk_rect(x, y, width, height),
                        to_nk_flags(flags)) != 0;
    }

    void ui::end_window()
    {
        nk_end(m_context);
    }

    void ui::layout_row_dynamic(float height, int cols)
    {
        nk_layout_row_dynamic(m_context, height, cols);
    }

    void ui::layout_row_static(float height, int item_width, int cols)
    {
        nk_layout_row_static(m_context, height, item_width, cols);
    }

    void ui::layout_row_begin(float height, int cols)
    {
        nk_layout_row_begin(m_context, NK_DYNAMIC, height, cols);
    }

    void ui::layout_row_push(float value)
    {
        nk_layout_row_push(m_context, value);
    }

    void ui::layout_row_end()
    {
        nk_layout_row_end(m_context);
    }

    bool ui::button(const char* label)
    {
        return nk_button_label(m_context, label) != 0;
    }

    void ui::label(const char* text, ui_alignment alignment)
    {
        nk_label(m_context, text, to_nk_flags(alignment));
    }

    void ui::label_colored(const char*  text,
                           ui_alignment alignment,
                           uint8_t      r,
                           uint8_t      g,
                           uint8_t      b,
                           uint8_t      a)
    {
        nk_label_colored(m_context,
                         text,
                         to_nk_flags(alignment),
                         nk_rgba(r, g, b, a));
    }

    void ui::label_wrap(const char* text)
    {
        nk_label_wrap(m_context, text);
    }

    bool ui::checkbox(const char* label, bool& active)
    {
        int state = active ? 1 : 0;
        int result = nk_checkbox_label(m_context, label, &state);
        active = state != 0;
        return result != 0;
    }

    bool ui::option(const char* label, bool active)
    {
        return nk_option_label(m_context, label, active ? 1 : 0) != 0;
    }

    bool
    ui::selectable(const char* label, ui_alignment alignment, bool& selected)
    {
        int val = selected ? 1 : 0;
        int result =
            nk_selectable_label(m_context, label, to_nk_flags(alignment), &val);
        selected = val != 0;
        return result != 0;
    }

    bool ui::slider(float min, float& value, float max, float step)
    {
        return nk_slider_float(m_context, min, &value, max, step) != 0;
    }

    bool ui::slider_int(int min, int& value, int max, int step)
    {
        return nk_slider_int(m_context, min, &value, max, step) != 0;
    }

    bool ui::progress(size_t& current, size_t max, bool modifiable)
    {
        nk_size cur = static_cast<nk_size>(current);
        int     result = nk_progress(m_context,
                                 &cur,
                                 static_cast<nk_size>(max),
                                 modifiable ? nk_true : nk_false);
        current = static_cast<size_t>(cur);
        return result != 0;
    }

    void ui::property_int(const char* name,
                          int         min,
                          int&        val,
                          int         max,
                          int         step,
                          float       inc_per_pixel)
    {
        nk_property_int(m_context, name, min, &val, max, step, inc_per_pixel);
    }

    void ui::property_float(const char* name,
                            float       min,
                            float&      val,
                            float       max,
                            float       step,
                            float       inc_per_pixel)
    {
        nk_property_float(m_context, name, min, &val, max, step, inc_per_pixel);
    }

    ui_edit_events ui::edit_string(char* buffer, int* length, int max_length)
    {
        nk_flags       result = nk_edit_string(m_context,
                                         NK_EDIT_SIMPLE,
                                         buffer,
                                         length,
                                         max_length,
                                         nk_filter_default);
        ui_edit_events events;
        for (uint32_t i = 0; i < static_cast<uint32_t>(ui_edit_event_flag::MAX);
             ++i) {
            if (result & (1u << i)) {
                events.set(static_cast<ui_edit_event_flag>(i));
            }
        }
        return events;
    }

    int ui::combo(const char* const* items,
                  int                count,
                  int                selected,
                  int                item_height,
                  float              width,
                  float              height)
    {
        return nk_combo(m_context,
                        items,
                        count,
                        selected,
                        item_height,
                        nk_vec2(width, height));
    }

    void ui::spacing(int cols)
    {
        nk_spacing(m_context, cols);
    }

    void ui::separator(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        nk_rule_horizontal(m_context, nk_rgba(r, g, b, a), nk_true);
    }

    bool ui::begin_group(const char* title, ui_window_flags flags)
    {
        return nk_group_begin(m_context, title, to_nk_flags(flags)) != 0;
    }

    void ui::end_group()
    {
        nk_group_end(m_context);
    }

    bool ui::tree_push(const char* title, bool initially_open)
    {
        return nk_tree_push_hashed(m_context,
                                   NK_TREE_TAB,
                                   title,
                                   initially_open ? NK_MAXIMIZED : NK_MINIMIZED,
                                   title,
                                   static_cast<int>(strlen(title)),
                                   0) != 0;
    }

    void ui::tree_pop()
    {
        nk_tree_pop(m_context);
    }

    bool ui::begin_popup(int         type,
                         const char* title,
                         uint32_t    flags,
                         float       x,
                         float       y,
                         float       width,
                         float       height)
    {
        return nk_popup_begin(m_context,
                              static_cast<enum nk_popup_type>(type),
                              title,
                              static_cast<nk_flags>(flags),
                              nk_rect(x, y, width, height)) != 0;
    }

    void ui::close_popup()
    {
        nk_popup_close(m_context);
    }

    void ui::end_popup()
    {
        nk_popup_end(m_context);
    }

    void ui::tooltip(const char* text)
    {
        nk_tooltip(m_context, text);
    }

    void ui::menubar_begin()
    {
        nk_menubar_begin(m_context);
    }

    void ui::menubar_end()
    {
        nk_menubar_end(m_context);
    }

    bool ui::menu_begin(const char*  label,
                        ui_alignment alignment,
                        float        width,
                        float        height)
    {
        return nk_menu_begin_label(m_context,
                                   label,
                                   to_nk_flags(alignment),
                                   nk_vec2(width, height)) != 0;
    }

    bool ui::menu_item(const char* label, ui_alignment alignment)
    {
        return nk_menu_item_label(m_context, label, to_nk_flags(alignment)) !=
               0;
    }

    void ui::menu_end()
    {
        nk_menu_end(m_context);
    }

    void ui::draw(mge::pass& pass)
    {

        // Create uniform block on first draw after program linking completes
        if (!m_uniform_block && !m_ui_program->needs_link()) {
            const auto& uniform_buffers = m_ui_program->uniform_buffers();
            if (!uniform_buffers.empty()) {
                m_uniform_block = new uniform_block(uniform_buffers[0]);

                // Set up orthographic projection matrix
                auto  extent = m_render_context->extent();
                float vp_width = static_cast<float>(extent.width);
                float vp_height = static_cast<float>(extent.height);

                // Orthographic projection: screen space (0,0 top-left) -> NDC
                float ortho[16] = {2.0f / vp_width,
                                   0.0f,
                                   0.0f,
                                   0.0f,
                                   0.0f,
                                   -2.0f / vp_height,
                                   0.0f,
                                   0.0f,
                                   0.0f,
                                   0.0f,
                                   -1.0f,
                                   0.0f,
                                   -1.0f,
                                   1.0f,
                                   0.0f,
                                   1.0f};
                m_uniform_block->set_data("projection", ortho, sizeof(ortho));
            }
        }

        // Skip drawing if uniform block not ready yet
        if (!m_uniform_block) {
            pass.touch();
            return;
        }

        // Convert nuklear draw commands to vertex/index buffers
        nk_buffer_clear(m_commands);
        nk_buffer_clear(m_vertices);
        nk_buffer_clear(m_indices);

        nk_flags convert_result = nk_convert(m_context,
                                             m_commands,
                                             m_vertices,
                                             m_indices,
                                             m_convert_config);

        if (convert_result != NK_CONVERT_SUCCESS) {
            MGE_WARNING_TRACE(UI,
                              "nk_convert failed with flags: {}",
                              static_cast<int>(convert_result));
        }

        // Get vertex and index data from nuklear buffers
        const void* vertices = nk_buffer_memory_const(m_vertices);
        nk_size     vertex_size = m_vertices->allocated;
        const void* indices = nk_buffer_memory_const(m_indices);
        nk_size     index_size = m_indices->allocated;

        if (vertex_size == 0 || index_size == 0) {
            // No commands to draw
            pass.touch();
            return;
        }

        // Update vertex and index buffers
        m_vertex_buffer->set_data(make_buffer(vertices, vertex_size));
        m_index_buffer->set_data(make_buffer(indices, index_size));

        // Get command buffer for drawing
        auto& cmd = m_render_context->command_buffer(false);

        // Bind uniform block with projection matrix
        cmd.bind_uniform_block(m_uniform_block);

        // Set pipeline state for UI rendering
        cmd.blend_function(blend_factor::SRC_ALPHA,
                           blend_factor::ONE_MINUS_SRC_ALPHA);
        cmd.blend_equation(blend_operation::ADD);
        cmd.depth_write(false);
        cmd.depth_test_function(test::ALWAYS);
        cmd.cull_face(cull_mode::NONE);

        // Bind font texture as default
        cmd.bind_texture(m_font_texture.get());

        // Iterate through nuklear draw commands
        const nk_draw_command* nk_cmd = nullptr;
        nk_size                offset = 0;
        nk_draw_foreach(nk_cmd, m_context, m_commands)
        {
            if (nk_cmd->elem_count == 0)
                continue;

            // Set scissor rectangle, clamping to valid range
            int32_t sx = static_cast<int32_t>(nk_cmd->clip_rect.x);
            int32_t sy = static_cast<int32_t>(nk_cmd->clip_rect.y);
            int32_t sw = static_cast<int32_t>(nk_cmd->clip_rect.w);
            int32_t sh = static_cast<int32_t>(nk_cmd->clip_rect.h);
            if (sx < 0) {
                sw += sx;
                sx = 0;
            }
            if (sy < 0) {
                sh += sy;
                sy = 0;
            }
            if (sw < 1)
                sw = 1;
            if (sh < 1)
                sh = 1;
            rectangle scissor(static_cast<uint32_t>(sx),
                              static_cast<uint32_t>(sy),
                              static_cast<uint32_t>(sx + sw),
                              static_cast<uint32_t>(sy + sh));
            cmd.set_scissor(scissor);

            // Bind texture (font or custom)
            cmd.bind_uniform_block(m_uniform_block);
            if (nk_cmd->texture.ptr) {
                texture* tex = reinterpret_cast<texture*>(nk_cmd->texture.ptr);
                cmd.bind_texture(tex);
            } else {
                cmd.bind_texture(m_font_texture.get());
            }

            // Submit draw command
            cmd.draw(m_ui_program,
                     m_vertex_buffer,
                     m_index_buffer,
                     static_cast<uint32_t>(nk_cmd->elem_count),
                     static_cast<uint32_t>(offset));

            offset += nk_cmd->elem_count;
        }

        // Submit command buffer to pass
        pass.submit(cmd);

        // Clear nuklear state and begin collecting input for next frame
        nk_clear(m_context);
        nk_input_begin(m_context);
    }

    void ui::attach(input_handler& handler)
    {
        detach();
        m_input_handler = &handler;
        m_key_action_handler_key = handler.add_key_action_handler(
            [this](key k, key_action action, const modifier& m) -> bool {
                return handle_key_action(k, action, m);
            });
        m_mouse_action_handler_key =
            handler.add_mouse_action_handler([this](uint32_t        button,
                                                    mouse_action    action,
                                                    const modifier& m,
                                                    uint32_t        x,
                                                    uint32_t        y) -> bool {
                return handle_mouse_action(button, action, m, x, y);
            });
        m_mouse_move_handler_key = handler.add_mouse_move_handler(
            [this](uint32_t x, uint32_t y) -> bool {
                return handle_mouse_move(x, y);
            });
        m_character_handler_key = handler.add_character_handler(
            [this](uint32_t ch) -> bool { return handle_character(ch); });
        m_mouse_wheel_handler_key = handler.add_mouse_wheel_handler(
            [this](int32_t x, int32_t y) -> bool {
                return handle_mouse_wheel(x, y);
            });
    }

    void ui::detach()
    {
        if (m_input_handler) {
            if (m_key_action_handler_key) {
                m_input_handler->remove_key_action_handler(
                    m_key_action_handler_key);
                m_key_action_handler_key = 0;
            }
            if (m_mouse_action_handler_key) {
                m_input_handler->remove_mouse_action_handler(
                    m_mouse_action_handler_key);
                m_mouse_action_handler_key = 0;
            }
            if (m_mouse_move_handler_key) {
                m_input_handler->remove_mouse_move_handler(
                    m_mouse_move_handler_key);
                m_mouse_move_handler_key = 0;
            }
            if (m_character_handler_key) {
                m_input_handler->remove_character_handler(
                    m_character_handler_key);
                m_character_handler_key = 0;
            }
            if (m_mouse_wheel_handler_key) {
                m_input_handler->remove_mouse_wheel_handler(
                    m_mouse_wheel_handler_key);
                m_mouse_wheel_handler_key = 0;
            }
            m_input_handler = nullptr;
        }
    }

    bool ui::handle_key_action(key k, key_action action, const modifier& m)
    {
        MGE_DEBUG_TRACE(UI,
                        "Key action: key={}, action={}",
                        static_cast<int>(k),
                        static_cast<int>(action));

        bool down =
            (action == key_action::PRESS || action == key_action::REPEAT);

        // Map MGE keys to Nuklear keys
        switch (k) {
        case key::BACKSPACE:
            nk_input_key(m_context, NK_KEY_BACKSPACE, down);
            break;
        case key::DELETE_KEY:
            nk_input_key(m_context, NK_KEY_DEL, down);
            break;
        case key::ENTER:
            nk_input_key(m_context, NK_KEY_ENTER, down);
            break;
        case key::TAB:
            nk_input_key(m_context, NK_KEY_TAB, down);
            break;
        case key::CURSOR_UP:
            nk_input_key(m_context, NK_KEY_UP, down);
            break;
        case key::CURSOR_DOWN:
            nk_input_key(m_context, NK_KEY_DOWN, down);
            break;
        case key::CURSOR_LEFT:
            nk_input_key(m_context, NK_KEY_LEFT, down);
            break;
        case key::CURSOR_RIGHT:
            nk_input_key(m_context, NK_KEY_RIGHT, down);
            break;
        case key::HOME:
            nk_input_key(m_context, NK_KEY_TEXT_START, down);
            break;
        case key::END:
            nk_input_key(m_context, NK_KEY_TEXT_END, down);
            break;
        default:
            break;
        }

        // Handle modifier keys
        if (m.test(modifier_value::SHIFT)) {
            nk_input_key(m_context, NK_KEY_SHIFT, down);
        }
        if (m.test(modifier_value::CONTROL)) {
            nk_input_key(m_context, NK_KEY_CTRL, down);
        }

        return true;
    }

    bool ui::handle_mouse_action(uint32_t        button,
                                 mouse_action    action,
                                 const modifier& m,
                                 uint32_t        x,
                                 uint32_t        y)
    {
        MGE_DEBUG_TRACE(UI,
                        "Mouse action: button={}, action={}, x={}, y={}",
                        button,
                        static_cast<int>(action),
                        x,
                        y);

        bool down = (action == mouse_action::PRESS);

        // Map button index to Nuklear button enum
        // Win32: 1=left, 2=right, 3=middle
        nk_buttons nk_button;
        switch (button) {
        case 1:
            nk_button = NK_BUTTON_LEFT;
            break;
        case 2:
            nk_button = NK_BUTTON_RIGHT;
            break;
        case 3:
            nk_button = NK_BUTTON_MIDDLE;
            break;
        default:
            return true;
        }

        nk_input_button(m_context,
                        nk_button,
                        static_cast<int>(x),
                        static_cast<int>(y),
                        down);

        return true;
    }

    bool ui::handle_mouse_move(uint32_t x, uint32_t y)
    {
        nk_input_motion(m_context, static_cast<int>(x), static_cast<int>(y));
        return true;
    }

    bool ui::handle_character(uint32_t ch)
    {
        nk_input_unicode(m_context, ch);
        return true;
    }

    bool ui::handle_mouse_wheel(int32_t x, int32_t y)
    {
        struct nk_vec2 scroll;
        scroll.x = static_cast<float>(x);
        scroll.y = static_cast<float>(y);
        nk_input_scroll(m_context, scroll);
        return true;
    }

} // namespace mge
