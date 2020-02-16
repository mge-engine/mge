// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/ui/dllexport.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/drawable.hpp"
#include "mge/input/input_handler.hpp"
#include "mge/ui/window.hpp"

namespace mge {
    namespace ui {
        
        /**
         * Context to envelope an UI subsystem. UI elements
         * belong to a context. 
         */
        class MGE_UI_EXPORT context 
            : public drawable
            , public std::enable_shared_from_this<mge::ui::context>
        {
        public:
            /**
             * Default constructor.
             */
            context();

            /**
             * Destructor. Detaches attached input, if any.
             */
            virtual ~context();

            void on_draw(render_context& render_context);

            /**
             * Attaches the ui context to an input handler.
             * 
             * @param input 
             */
            void attach_input(input_handler& input);

            /**
             * Detaches current input.
             */
            void detach_input();

            /**
             * Create a window.
             * @return created window
             */
            window_ref create_window();
        private:
            input_handler *m_input;
        };
    }
}
