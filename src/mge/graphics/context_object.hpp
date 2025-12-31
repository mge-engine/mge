// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/noncopyable.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

namespace mge {

    /**
     * An object that is valid in the context of a @c render_context like
     * a shader program, a texture, or a buffer.
     */
    class MGEGRAPHICS_EXPORT context_object : public noncopyable
    {
    protected:
        /**
         * @brief Construct a new context object object.
         * @param context   render context
         */
        explicit context_object(render_context& context);

    public:
        /**
         * @brief Destroy the context object.
         */
        virtual ~context_object();
        /**
         * @brief Get the render context.
         */
        const render_context& context() const;
        /**
         * @brief Get the render context.
         */
        render_context& context();

        /**
         * @brief Assert that the given render context is the same
         * as the one of this context object.
         * @param c render context to compare
         * @throw illegal_state if the contexts differ
         */
        void assert_same_context(const render_context& c) const;

        /**
         * @brief Assert that the given context object is in the same
         * render context as this context object.
         * @param cobj context object to compare
         * @throw illegal_state if the contexts differ
         */
        void assert_same_context(const context_object& cobj) const;
        /**
         * @brief Assert that the given context object is in the same
         * render context as this context object.
         * @param cobj context object to compare
         * @throw illegal_state if the contexts differ
         */
        void
        assert_same_context(const std::shared_ptr<context_object>& cobj) const;

        /**
         * @brief Check whether the context object is ready to be used.
         *
         * @return @c true if the context object is ready
         */
        bool ready() const noexcept
        {
            return m_ready;
        }

        /**
         * @brief Assert that the context object is ready to be used.
         *
         * @throw illegal_state if the context object is not ready
         */
        void assert_ready() const;

    protected:
        /**
         * @brief Set the ready state of the context object.
         * @param r ready state
         */
        void set_ready(bool r) noexcept
        {
            m_ready = r;
        }

        render_context& m_context;
        bool            m_ready;
    };

} // namespace mge