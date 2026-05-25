// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/tuple_hash.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "vulkan.hpp"

#include <unordered_map>

namespace mge::vulkan {
    class render_system;
    class vertex_buffer;
    class program;

    class render_context_base : public mge::render_context
    {
    public:
        virtual ~render_context_base();

        mge::index_buffer*  on_create_index_buffer(data_type dt,
                                                   size_t    data_size) override;
        mge::vertex_buffer* on_create_vertex_buffer(const vertex_layout& layout,
                                                    size_t data_size) override;
        mge::shader*        on_create_shader(shader_type t) override;
        mge::program*       on_create_program() override;
        mge::texture_ref    create_texture(texture_type type) override;
        mge::texture_ref
        create_render_target_texture(texture_type        type,
                                     const image_format& format,
                                     const mge::extent&  extent) override;
        mge::frame_buffer*
        on_create_frame_buffer(const mge::frame_buffer_info& info) override;

#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define DEVICE_FUNCTION(X) PFN_##X X{nullptr};

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

        VkDevice device() const noexcept
        {
            return m_device;
        }

        VmaAllocator allocator() const noexcept
        {
            return m_allocator;
        }

        VkQueue graphics_queue() const noexcept
        {
            return m_queue;
        }

        VkCommandPool graphics_command_pool() const noexcept
        {
            return m_graphics_command_pool;
        }

        VkFormat depth_format() const noexcept
        {
            return m_depth_format;
        }

        const std::vector<VkVertexInputAttributeDescription>&
        vertex_input_attribute_descriptions(const mge::vertex_layout& layout);

        VkPipeline pipeline(const vertex_buffer&       buffer,
                            const program&             prog,
                            const mge::pipeline_state& state,
                            VkRenderPass               render_pass);

        void draw_geometry(VkCommandBuffer                  command_buffer,
                           mge::program*                    prog,
                           mge::vertex_buffer*              vb,
                           mge::index_buffer*               ib,
                           const mge::pipeline_state&       state,
                           mge::uniform_block*              ub,
                           const mge::texture_binding_list& textures,
                           VkRenderPass                     render_pass,
                           uint32_t                         index_count = 0,
                           uint32_t                         index_offset = 0);

        void bind_uniform_block(VkCommandBuffer       command_buffer,
                                mge::vulkan::program& vk_program,
                                mge::uniform_block&   ub);

        void bind_texture(VkCommandBuffer       command_buffer,
                          mge::vulkan::program& vk_program,
                          mge::texture*         tex,
                          mge::uniform_block*   ub = nullptr);

        VkDescriptorSet prepare_uniform_block(mge::vulkan::program& vk_program,
                                              mge::uniform_block&   ub);

        VkDescriptorSet
        prepare_texture(mge::vulkan::program&            vk_program,
                        const mge::texture_binding_list& textures,
                        mge::uniform_block*              ub);

    protected:
        render_context_base(mge::vulkan::render_system& rs,
                            const mge::extent&          ext);

        virtual std::vector<const char*> get_device_extensions() const;

        void create_device();
        void create_allocator();
        void get_device_queue();
        void resolve_device_functions();
        void clear_functions();
        void create_graphics_command_pool();
        void create_descriptor_pool();
        void init_capabilities();
        void find_depth_format();
        void teardown_shared();

        void record_render_pass(const mge::pass& p,
                                VkRenderPass     render_pass,
                                VkFramebuffer    framebuffer,
                                VkExtent2D       extent,
                                VkCommandBuffer  command_buffer);

        std::shared_ptr<mge::vulkan::render_system> m_render_system;
        VkDevice                                    m_device{VK_NULL_HANDLE};
        VmaAllocator                                m_allocator{VK_NULL_HANDLE};
        VkQueue                                     m_queue{VK_NULL_HANDLE};
        VkCommandPool            m_graphics_command_pool{VK_NULL_HANDLE};
        VkDescriptorPool         m_descriptor_pool{VK_NULL_HANDLE};
        VkFormat                 m_depth_format{VK_FORMAT_UNDEFINED};

        struct uniform_buffer_data
        {
            VkBuffer      buffer{VK_NULL_HANDLE};
            VmaAllocation allocation{VK_NULL_HANDLE};
            uint64_t      version{0};
            void*         mapped_data{nullptr};
        };
        std::map<mge::uniform_block*, uniform_buffer_data> m_uniform_buffers;
        using descriptor_set_key = std::tuple<mge::uniform_block*,
                                              std::vector<mge::texture*>,
                                              VkDescriptorSetLayout>;
        std::map<descriptor_set_key, VkDescriptorSet> m_descriptor_sets;

        using pipeline_key_type =
            std::tuple<VkBuffer, VkPipelineLayout, mge::pipeline_state,
                       VkRenderPass>;
        using pipeline_cache_type =
            std::unordered_map<pipeline_key_type, VkPipeline>;
        pipeline_cache_type m_pipelines;

        std::unordered_map<mge::vertex_layout,
                           std::vector<VkVertexInputAttributeDescription>>
            m_vertex_input_attribute_descriptions;
    };

} // namespace mge::vulkan
