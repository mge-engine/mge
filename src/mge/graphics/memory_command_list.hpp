// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include <variant>
#include <vector>

namespace mge {

    /**
     * An abstract platform independent command list.
     *
     * This command list just collects the commands issued, but does
     * not use any native functionality. Executing such a command list
     * may impose a performance penalty.
     */
    class MGE_GRAPHICS_EXPORT memory_command_list : public command_list
    {
    public:
        struct clear_data {
            rgba_color color;
        };

        struct clear_depth_data {
            double value;
        };

        struct clear_stencil_data {
            int32_t value;
        };

        struct set_pipeline_data {
            pipeline_ref pipeline;
        };

        struct play_command_list_data {
            command_list_ref commands;
        };

        struct draw_data {
            vertex_buffer_ref vertices;
            index_buffer_ref indices;
        };

        using element_type = std::variant<clear_data,
                                          clear_depth_data,
                                          clear_stencil_data,
                                          set_pipeline_data,
                                          play_command_list_data,
                                          draw_data>;

        using container_type = std::vector<element_type>;
        using const_iterator = container_type::const_iterator;

        /**
         * Constructor.
         * @param context render context
         */
        memory_command_list(render_context& context);

        /**
         * Destructor.
         */
        ~memory_command_list() = default;

        inline void clear() { mge::command_list::clear(); }
        virtual void clear(const rgba_color& c) override;
        virtual void clear_depth(float value) override;
        virtual void clear_stencil(int32_t value) override;
        virtual void draw(const vertex_buffer_ref &vertices, const index_buffer_ref &indices) override;

        inline const_iterator begin() const { return m_elements.begin(); }
        inline const_iterator end() const { return m_elements.end(); }
    protected:
        void on_finish() override;
        void on_set_pipeline(const pipeline_ref& pipeline);
        void on_play(const command_list_ref &commands) override;
        void on_clear() override;
    private:
        container_type m_elements;
    };
}
