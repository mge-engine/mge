// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/rgba_color.hpp"

namespace mge {

    /**
     * A command list ist a list of state changing commands which
     * can be executed by a render context. A command list can be
     * either implemented natively, i.e. the graphics subsystem
     * implements command queues and command lists, or abstract.
     */
    class MGE_GRAPHICS_EXPORT command_list
            : public context_object
    {
    protected:
        /**
         * Constructor.
         * @param context   render context
         * @param native    flag whether this command list is native
         *                  or abstract
         */
        command_list(render_context& context, bool native=true);
    public:
        /**
         * Destructor.
         */
        virtual ~command_list() = default;

        /**
         * Get whether this is a native command list.
         * @return @c true if native
         */
        bool native() const noexcept { return m_native; }

        /**
         * Schedules a clear command of the color buffer.
         * @param c color to use for clearing
         */
        virtual void clear(const rgba_color& c) = 0;

        /**
         * Schedule a clear depth command.
         * 
         * @param depth depth value to use for clearing
         */
        virtual void clear_depth(float depth) = 0;

        /**
         * Schedule a clear stencil command.
         * 
         * @param depth depth value to use for clearing
         */
        virtual void clear_stencil(int32_t value) = 0;

        /**
         * Apply a pipeline. Current pipeline is replaced
         * by this pipeline.
         * 
         * @param pipeline pipeline to be applied
         */
        void set_pipeline(const pipeline_ref& pipeline);

        /**
         * Play a command list within the command list.
         * @param commands commands to play
         */
        void play(const command_list_ref& commands);

        /**
         * Empties the command list and brings it back to
         * recording state.
         */
        void clear();

        /**
         * Finishes recording of commands. The command list is immutable
         * afterwards.
         */
        void finish();

        /**
         * Return whether this command list has finished recording.
         * 
         * @return @c true if command list has been finished
         */
        bool immutable() const noexcept { return m_finished; }

        /**
         * Return whether this command list has finished recording.
         * 
         * @return @c true if command list has been finished
         */
        bool finished() const noexcept { return m_finished; }

    protected:
        /**
         * Assert the command list can still record commands.
         */
        void assert_not_immutable();

        virtual void on_finish() = 0;
        virtual void on_set_pipeline(const pipeline_ref& p) = 0;
        virtual void on_play(const command_list_ref& commands) = 0;
        virtual void on_clear() = 0;
    private:
        pipeline_ref m_current_pipeline;
        bool         m_native;
        bool         m_finished;
    };
}
