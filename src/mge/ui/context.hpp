#pragma once
#include "mge/ui/dllexport.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/drawable.hpp"
#include "mge/input/input_handler.hpp"

namespace mge {
    namespace ui {
        
        /**
         * Context to envelope an UI subsystem. UI elements
         * belong to a context. 
         */
        class MGE_UI_EXPORT context 
            : public drawable
            , std::enable_shared_from_this<context>
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

            void draw(render_context& render_context) override;

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
        private:
            input_handler *m_input;
        };
    }
}