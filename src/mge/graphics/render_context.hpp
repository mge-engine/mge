// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/buffer_access.hpp"
#include "mge/graphics/buffer_usage.hpp"
#include "mge/core/async_executor.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/shader_language.hpp"
#include <memory>
#include <vector>
namespace mge {

    /**
     * A render context is a context in which objects that are elements
     * of the render pipeline are created - such as shader programs,
     * textures and buffers.
     */
    class MGE_GRAPHICS_EXPORT render_context
            : public std::enable_shared_from_this<render_context>
            , public async_executor
    {
    protected:
        render_context(async_executor *display_executor);
    public:
        virtual ~render_context() = default;
        void await(const std::function<void()>& f) override;

        /**
         * Assigns the render context to this thread, in case of the
         * underlying API needs such an assignment.
         */
        virtual void assign_thread();

        /**
         * Flush outstanding work, and return when work has been
         * presented.
         */
        virtual void flush() = 0;

        /**
         * Create an index buffer.
         * @param usage         buffer usage
         * @param cpu_access    cpu access flags
         * @param gpu_access    gpu access flags
         * @return index buffer
         */
        virtual index_buffer_ref create_index_buffer(buffer_usage usage,
                                                     buffer_access cpu_access,
                                                     buffer_access gpu_access) = 0;

        /**
         * Create an index buffer.
         * @param usage         buffer usage
         * @param cpu_access    cpu access flags
         * @param gpu_access    gpu access flags
         * @param data          buffer data
         * @param data_size     buffer size
         */
        virtual index_buffer_ref create_index_buffer(buffer_usage usage,
                                                     buffer_access cpu_access,
                                                     buffer_access gpu_access,
                                                     void *data,
                                                     size_t data_size) = 0;

        /**
         * Get supported shader languages.
         * @param languages vector of languages
         */
        virtual void shader_languages(std::vector<shader_language>& languages) const = 0;

    protected:
        async_executor *m_display_executor;
    };
}
