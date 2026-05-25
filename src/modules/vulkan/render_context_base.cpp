// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "render_context_base.hpp"
#include "enumerate.hpp"
#include "error.hpp"
#include "frame_buffer.hpp"
#include "index_buffer.hpp"
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"

#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    static inline VkBlendFactor blend_factor_to_vulkan(blend_factor factor)
    {
        switch (factor) {
        case blend_factor::ZERO:
            return VK_BLEND_FACTOR_ZERO;
        case blend_factor::ONE:
            return VK_BLEND_FACTOR_ONE;
        case blend_factor::SRC_COLOR:
            return VK_BLEND_FACTOR_SRC_COLOR;
        case blend_factor::ONE_MINUS_SRC_COLOR:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case blend_factor::DST_COLOR:
            return VK_BLEND_FACTOR_DST_COLOR;
        case blend_factor::ONE_MINUS_DST_COLOR:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case blend_factor::SRC_ALPHA:
            return VK_BLEND_FACTOR_SRC_ALPHA;
        case blend_factor::ONE_MINUS_SRC_ALPHA:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case blend_factor::DST_ALPHA:
            return VK_BLEND_FACTOR_DST_ALPHA;
        case blend_factor::ONE_MINUS_DST_ALPHA:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case blend_factor::CONSTANT_COLOR:
            return VK_BLEND_FACTOR_CONSTANT_COLOR;
        case blend_factor::ONE_MINUS_CONSTANT_COLOR:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        case blend_factor::CONSTANT_ALPHA:
            return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case blend_factor::ONE_MINUS_CONSTANT_ALPHA:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        case blend_factor::SRC_ALPHA_SATURATE:
            return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
        case blend_factor::SRC1_COLOR:
            return VK_BLEND_FACTOR_SRC1_COLOR;
        case blend_factor::ONE_MINUS_SRC1_COLOR:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
        case blend_factor::SRC1_ALPHA:
            return VK_BLEND_FACTOR_SRC1_ALPHA;
        case blend_factor::ONE_MINUS_SRC1_ALPHA:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend factor: " << factor;
        }
    }

    static inline VkBlendOp blend_operation_to_vulkan(blend_operation op)
    {
        switch (op) {
        case blend_operation::NONE:
            return VK_BLEND_OP_ADD;
        case blend_operation::ADD:
            return VK_BLEND_OP_ADD;
        case blend_operation::SUBTRACT:
            return VK_BLEND_OP_SUBTRACT;
        case blend_operation::REVERSE_SUBTRACT:
            return VK_BLEND_OP_REVERSE_SUBTRACT;
        case blend_operation::MIN:
            return VK_BLEND_OP_MIN;
        case blend_operation::MAX:
            return VK_BLEND_OP_MAX;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend operation: " << op;
        }
    }

    static inline VkCompareOp depth_test_to_vulkan(mge::test func)
    {
        switch (func) {
        case mge::test::NEVER:
            return VK_COMPARE_OP_NEVER;
        case mge::test::LESS:
            return VK_COMPARE_OP_LESS;
        case mge::test::EQUAL:
            return VK_COMPARE_OP_EQUAL;
        case mge::test::LESS_EQUAL:
            return VK_COMPARE_OP_LESS_OR_EQUAL;
        case mge::test::GREATER:
            return VK_COMPARE_OP_GREATER;
        case mge::test::NOT_EQUAL:
            return VK_COMPARE_OP_NOT_EQUAL;
        case mge::test::GREATER_EQUAL:
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case mge::test::ALWAYS:
            return VK_COMPARE_OP_ALWAYS;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown depth test: " << static_cast<int>(func);
        }
    }

    render_context_base::render_context_base(mge::vulkan::render_system& rs,
                                             const mge::extent&          ext)
        : mge::render_context(rs, ext)
        , m_render_system(rs.shared_from_this())
    {}

    render_context_base::~render_context_base() {}

    mge::index_buffer*
    render_context_base::on_create_index_buffer(data_type dt, size_t data_size)
    {
        return new index_buffer(*this, dt, data_size);
    }

    mge::vertex_buffer* render_context_base::on_create_vertex_buffer(
        const vertex_layout& layout, size_t data_size)
    {
        return new mge::vulkan::vertex_buffer(*this, layout, data_size);
    }

    mge::shader* render_context_base::on_create_shader(shader_type t)
    {
        return new shader(*this, t);
    }

    mge::program* render_context_base::on_create_program()
    {
        return new mge::vulkan::program(*this);
    }

    mge::texture_ref render_context_base::create_texture(texture_type type)
    {
        return std::make_shared<mge::vulkan::texture>(*this, type);
    }

    mge::texture_ref render_context_base::create_render_target_texture(
        texture_type type, const image_format& format, const mge::extent& extent)
    {
        return std::make_shared<mge::vulkan::texture>(*this, type, format,
                                                      extent);
    }

    mge::frame_buffer* render_context_base::on_create_frame_buffer(
        const mge::frame_buffer_info& info)
    {
        return new mge::vulkan::frame_buffer(*this, info);
    }

    std::vector<const char*> render_context_base::get_device_extensions() const
    {
        return {};
    }

    void render_context_base::create_device()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create logical device");
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex =
            m_render_system->graphics_queue_index();
        queue_create_info.queueCount = 1;
        float queue_priority = 1.0f;
        queue_create_info.pQueuePriorities = &queue_priority;

        VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo device_create_info{};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos = &queue_create_info;
        device_create_info.queueCreateInfoCount = 1;
        device_create_info.pEnabledFeatures = &device_features;

        std::vector<const char*> device_extensions = get_device_extensions();
#ifdef MGE_OS_MACOSX
        device_extensions.push_back("VK_KHR_portability_subset");
#endif

        std::vector<const char*> device_layers;
        if (m_render_system->debug()) {
            device_layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        device_create_info.ppEnabledLayerNames = device_layers.data();
        device_create_info.enabledLayerCount =
            static_cast<uint32_t>(device_layers.size());
        device_create_info.enabledExtensionCount =
            static_cast<uint32_t>(device_extensions.size());
        device_create_info.ppEnabledExtensionNames = device_extensions.data();

        CHECK_VK_CALL(
            m_render_system->vkCreateDevice(m_render_system->physical_device(),
                                            &device_create_info,
                                            nullptr,
                                            &m_device));
        MGE_DEBUG_TRACE(VULKAN,
                        "Created logical device: {}",
                        static_cast<void*>(m_device));
    }

    static void* resolve_device_function(void*                   original,
                                         PFN_vkGetDeviceProcAddr getDeviceProc,
                                         VkDevice                device,
                                         const char*             name)
    {
        void* result = original;
        auto  ptr = getDeviceProc(device, name);
        if (ptr) {
            result = reinterpret_cast<void*>(ptr);
        }
        return result;
    }

    void render_context_base::resolve_device_functions()
    {
        MGE_DEBUG_TRACE(VULKAN, "Resolve device functions");
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X)                                                             \
    this->X = reinterpret_cast<decltype(this->X)>(                             \
        resolve_device_function((void*)m_render_system->X,                     \
                                m_render_system->vkGetDeviceProcAddr,          \
                                m_device,                                      \
                                #X));

#define BASIC_INSTANCE_FUNCTION(X)
#define INSTANCE_FUNCTION(X)
#define DEVICE_FUNCTION(X) RESOLVE(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#    pragma warning(pop)
#endif
    }

    void render_context_base::clear_functions()
    {
        MGE_DEBUG_TRACE(VULKAN, "Clear device functions");
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X) this->X = nullptr;

#define BASIC_INSTANCE_FUNCTION(X) RESOLVE(X)
#define INSTANCE_FUNCTION(X) RESOLVE(X)
#define DEVICE_FUNCTION(X) RESOLVE(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#    pragma warning(pop)
#endif
    }

    void render_context_base::get_device_queue()
    {
        MGE_DEBUG_TRACE(VULKAN, "Get device queue");
        vkGetDeviceQueue(m_device,
                         m_render_system->graphics_queue_index(),
                         0,
                         &m_queue);
    }

    void render_context_base::create_allocator()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create allocator");
        VmaVulkanFunctions vk_functions = {};
        vk_functions.vkGetInstanceProcAddr =
            m_render_system->library().vkGetInstanceProcAddr;
        vk_functions.vkGetDeviceProcAddr = m_render_system->vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo allocator_info = {};
        allocator_info.physicalDevice = m_render_system->physical_device();
        allocator_info.device = m_device;
        allocator_info.instance = m_render_system->instance();
        allocator_info.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
        allocator_info.pVulkanFunctions = &vk_functions;
        allocator_info.vulkanApiVersion = VK_API_VERSION_1_3;
        CHECK_VK_CALL(vmaCreateAllocator(&allocator_info, &m_allocator));
    }

    void render_context_base::create_graphics_command_pool()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create graphics command pool");
        VkCommandPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.queueFamilyIndex = m_render_system->graphics_queue_index();
        pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
                          VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        CHECK_VK_CALL(vkCreateCommandPool(m_device,
                                          &pool_info,
                                          nullptr,
                                          &m_graphics_command_pool));
    }

    void render_context_base::create_descriptor_pool()
    {
        MGE_DEBUG_TRACE(VULKAN, "Create descriptor pool");

        std::array<VkDescriptorPoolSize, 2> pool_sizes = {};
        pool_sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pool_sizes[0].descriptorCount = 1000;
        pool_sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[1].descriptorCount = 1000;

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
        pool_info.pPoolSizes = pool_sizes.data();
        pool_info.maxSets = 1000;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        CHECK_VK_CALL(vkCreateDescriptorPool(m_device,
                                             &pool_info,
                                             nullptr,
                                             &m_descriptor_pool));
    }

    void render_context_base::init_capabilities()
    {
        VkPhysicalDeviceProperties props;
        m_render_system->vkGetPhysicalDeviceProperties(
            m_render_system->physical_device(),
            &props);
        const VkPhysicalDeviceLimits& limits = props.limits;

        class capabilities : public mge::render_context::capabilities
        {
        public:
            explicit capabilities(const VkPhysicalDeviceLimits& l)
            {
                m_max_texture_size = l.maxImageDimension2D;
                m_max_texture_3d_size = l.maxImageDimension3D;
                m_max_texture_cube_size = l.maxImageDimensionCube;
                m_max_texture_array_layers = l.maxImageArrayLayers;
                m_max_vertex_attributes = l.maxVertexInputAttributes;
                m_max_uniform_buffer_bindings =
                    l.maxDescriptorSetUniformBuffers;
                m_max_texture_bindings = l.maxDescriptorSetSampledImages;
                m_max_color_attachments = l.maxColorAttachments;
            }
            ~capabilities() = default;

            uint32_t max_texture_size() const override
            {
                return m_max_texture_size;
            }

            uint32_t max_texture_3d_size() const override
            {
                return m_max_texture_3d_size;
            }

            uint32_t max_texture_cube_size() const override
            {
                return m_max_texture_cube_size;
            }

            uint32_t max_texture_array_layers() const override
            {
                return m_max_texture_array_layers;
            }

            uint32_t max_vertex_attributes() const override
            {
                return m_max_vertex_attributes;
            }

            uint32_t max_uniform_buffer_bindings() const override
            {
                return m_max_uniform_buffer_bindings;
            }

            uint32_t max_texture_bindings() const override
            {
                return m_max_texture_bindings;
            }

            uint32_t max_color_attachments() const override
            {
                return m_max_color_attachments;
            }

        private:
            uint32_t m_max_texture_size{0};
            uint32_t m_max_texture_3d_size{0};
            uint32_t m_max_texture_cube_size{0};
            uint32_t m_max_texture_array_layers{0};
            uint32_t m_max_vertex_attributes{0};
            uint32_t m_max_uniform_buffer_bindings{0};
            uint32_t m_max_texture_bindings{0};
            uint32_t m_max_color_attachments{0};
        };

        m_capabilities = std::make_unique<capabilities>(limits);
    }

    void render_context_base::find_depth_format()
    {
        MGE_DEBUG_TRACE(VULKAN, "Find depth format");
        const VkFormat candidates[] = {
            VK_FORMAT_D24_UNORM_S8_UINT,
            VK_FORMAT_D32_SFLOAT,
        };

        for (auto format : candidates) {
            VkFormatProperties props;
            m_render_system->vkGetPhysicalDeviceFormatProperties(
                m_render_system->physical_device(),
                format,
                &props);
            if (props.optimalTilingFeatures &
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
                m_depth_format = format;
                MGE_DEBUG_TRACE(VULKAN,
                                "Selected depth format: {}",
                                static_cast<int>(format));
                return;
            }
        }
        MGE_THROW(mge::vulkan::error) << "No supported depth format found";
    }

    void render_context_base::teardown_shared()
    {
        for (auto& [ub, data] : m_uniform_buffers) {
            if (data.buffer != VK_NULL_HANDLE) {
                vmaDestroyBuffer(m_allocator, data.buffer, data.allocation);
            }
        }
        m_uniform_buffers.clear();

        if (vkDestroyDescriptorPool && m_descriptor_pool) {
            vkDestroyDescriptorPool(m_device, m_descriptor_pool, nullptr);
            m_descriptor_pool = VK_NULL_HANDLE;
        }
        m_descriptor_sets.clear();

        if (vkDestroyCommandPool && m_graphics_command_pool) {
            vkDestroyCommandPool(m_device, m_graphics_command_pool, nullptr);
            m_graphics_command_pool = VK_NULL_HANDLE;
        }

        m_queue = VK_NULL_HANDLE;

        if (m_allocator) {
            vmaDestroyAllocator(m_allocator);
            m_allocator = VK_NULL_HANDLE;
        }

        if (m_device && vkDestroyDevice) {
            vkDestroyDevice(m_device, nullptr);
            m_device = VK_NULL_HANDLE;
        }

        clear_functions();
    }

    void render_context_base::record_render_pass(const mge::pass& p,
                                                 VkRenderPass     render_pass,
                                                 VkFramebuffer    framebuffer,
                                                 VkExtent2D       pass_extent,
                                                 VkCommandBuffer  command_buffer)
    {
        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType      = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = render_pass;
        render_pass_info.framebuffer = framebuffer;
        render_pass_info.renderArea.offset = {0, 0};
        render_pass_info.renderArea.extent = pass_extent;

        vkCmdBeginRenderPass(command_buffer,
                             &render_pass_info,
                             VK_SUBPASS_CONTENTS_INLINE);

        const auto& vp = p.viewport();
        VkViewport  viewport{};
        viewport.x = static_cast<float>(vp.x);
        viewport.y = static_cast<float>(vp.y) + static_cast<float>(vp.height);
        viewport.width    = static_cast<float>(vp.width);
        viewport.height   = -static_cast<float>(vp.height);
        viewport.minDepth = vp.min_depth;
        viewport.maxDepth = vp.max_depth;
        vkCmdSetViewport(command_buffer, 0, 1, &viewport);

        const auto& sr = p.scissor();
        VkRect2D    scissor{};
        scissor.offset = {static_cast<int32_t>(sr.left),
                          static_cast<int32_t>(sr.top)};
        scissor.extent = {static_cast<uint32_t>(sr.right - sr.left),
                          static_cast<uint32_t>(sr.bottom - sr.top)};
        vkCmdSetScissor(command_buffer, 0, 1, &scissor);

        if (p.clear_color_enabled()) {
            const auto&  c = p.clear_color_value();
            VkClearValue clear_color = {};
            clear_color.color = {{c.r, c.g, c.b, c.a}};
            VkClearAttachment clear_attachment = {};
            clear_attachment.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            clear_attachment.colorAttachment = 0;
            clear_attachment.clearValue      = clear_color;
            VkClearRect clear_rect = {};
            clear_rect.rect.offset    = {0, 0};
            clear_rect.rect.extent    = pass_extent;
            clear_rect.baseArrayLayer = 0;
            clear_rect.layerCount     = 1;
            vkCmdClearAttachments(command_buffer, 1, &clear_attachment, 1,
                                  &clear_rect);
        }

        if (p.clear_depth_enabled() || p.clear_stencil_enabled()) {
            VkClearValue      clear_depth_stencil = {};
            VkClearAttachment clear_attachment    = {};
            if (p.clear_depth_enabled()) {
                clear_depth_stencil.depthStencil.depth = p.clear_depth_value();
                clear_attachment.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
            }
            if (p.clear_stencil_enabled()) {
                clear_depth_stencil.depthStencil.stencil =
                    p.clear_stencil_value();
                clear_attachment.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
            clear_attachment.clearValue = clear_depth_stencil;
            VkClearRect clear_rect = {};
            clear_rect.rect.offset    = {0, 0};
            clear_rect.rect.extent    = pass_extent;
            clear_rect.baseArrayLayer = 0;
            clear_rect.layerCount     = 1;
            vkCmdClearAttachments(command_buffer, 1, &clear_attachment, 1,
                                  &clear_rect);
        }

        bool           blend_pass_needed = false;
        mge::rectangle current_scissor   = p.scissor();
        for_each_draw_in_pass(
            p.index(),
            [this,
             command_buffer,
             render_pass,
             &blend_pass_needed,
             &current_scissor,
             &p](const program_handle&            prog,
                 const vertex_buffer_handle&      vertex_buffer,
                 const index_buffer_handle&       index_buffer,
                 const mge::pipeline_state&       state,
                 mge::uniform_block*              ub,
                 const mge::texture_binding_list& textures,
                 uint32_t                         index_count,
                 uint32_t                         index_offset,
                 const mge::rectangle&            cmd_scissor) {
                const auto& effective =
                    cmd_scissor.area() != 0 ? cmd_scissor : p.scissor();
                if (effective != current_scissor) {
                    VkRect2D vk_scissor{};
                    vk_scissor.offset = {
                        static_cast<int32_t>(effective.left),
                        static_cast<int32_t>(effective.top)};
                    vk_scissor.extent = {
                        static_cast<uint32_t>(effective.right -
                                              effective.left),
                        static_cast<uint32_t>(effective.bottom -
                                              effective.top)};
                    vkCmdSetScissor(command_buffer, 0, 1, &vk_scissor);
                    current_scissor = effective;
                }
                if (state.color_blend_operation() == mge::blend_operation::NONE) {
                    draw_geometry(command_buffer,
                                  prog.get(),
                                  vertex_buffer.get(),
                                  index_buffer.get(),
                                  state,
                                  ub,
                                  textures,
                                  render_pass,
                                  index_count,
                                  index_offset);
                } else {
                    blend_pass_needed = true;
                }
            });

        if (blend_pass_needed) {
            for_each_draw_in_pass(
                p.index(),
                [this,
                 command_buffer,
                 render_pass,
                 &current_scissor,
                 &p](const program_handle&            prog,
                     const vertex_buffer_handle&      vertex_buffer,
                     const index_buffer_handle&       index_buffer,
                     const mge::pipeline_state&       state,
                     mge::uniform_block*              ub,
                     const mge::texture_binding_list& textures,
                     uint32_t                         index_count,
                     uint32_t                         index_offset,
                     const mge::rectangle&            cmd_scissor) {
                    const auto& effective =
                        cmd_scissor.area() != 0 ? cmd_scissor : p.scissor();
                    if (effective != current_scissor) {
                        VkRect2D vk_scissor{};
                        vk_scissor.offset = {
                            static_cast<int32_t>(effective.left),
                            static_cast<int32_t>(effective.top)};
                        vk_scissor.extent = {
                            static_cast<uint32_t>(effective.right -
                                                  effective.left),
                            static_cast<uint32_t>(effective.bottom -
                                                  effective.top)};
                        vkCmdSetScissor(command_buffer, 0, 1, &vk_scissor);
                        current_scissor = effective;
                    }
                    draw_geometry(command_buffer,
                                  prog.get(),
                                  vertex_buffer.get(),
                                  index_buffer.get(),
                                  state,
                                  ub,
                                  textures,
                                  render_pass,
                                  index_count,
                                  index_offset);
                });
        }

        vkCmdEndRenderPass(command_buffer);
    }

    void render_context_base::bind_uniform_block(
        VkCommandBuffer       command_buffer,
        mge::vulkan::program& vk_program,
        mge::uniform_block&   ub)
    {
        VkDescriptorSet descriptor_set = prepare_uniform_block(vk_program, ub);
        if (descriptor_set != VK_NULL_HANDLE) {
            vkCmdBindDescriptorSets(command_buffer,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    vk_program.pipeline_layout(),
                                    0,
                                    1,
                                    &descriptor_set,
                                    0,
                                    nullptr);
        }
    }

    VkDescriptorSet render_context_base::prepare_uniform_block(
        mge::vulkan::program& vk_program, mge::uniform_block& ub)
    {
        ub.sync_from_globals();

        auto                 it = m_uniform_buffers.find(&ub);
        uniform_buffer_data* ub_data = nullptr;

        if (it != m_uniform_buffers.end()) {
            ub_data = &it->second;
        } else {
            size_t aligned_size = (ub.data_size() + 255) & ~255;

            VkBufferCreateInfo buffer_info = {};
            buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_info.size = aligned_size;
            buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo alloc_info = {};
            alloc_info.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
            alloc_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

            uniform_buffer_data new_data;
            VmaAllocationInfo   allocation_info = {};

            VkResult result = vmaCreateBuffer(m_allocator,
                                              &buffer_info,
                                              &alloc_info,
                                              &new_data.buffer,
                                              &new_data.allocation,
                                              &allocation_info);
            if (result != VK_SUCCESS) {
                MGE_ERROR_TRACE(VULKAN, "Failed to create uniform buffer");
                return VK_NULL_HANDLE;
            }

            new_data.mapped_data = allocation_info.pMappedData;
            new_data.version = 0;

            m_uniform_buffers[&ub] = new_data;
            ub_data = &m_uniform_buffers[&ub];
        }

        if (ub_data->version != ub.version()) {
            if (ub_data->mapped_data) {
                memcpy(ub_data->mapped_data, ub.data(), ub.data_size());
                CHECK_VK_CALL(vmaFlushAllocation(m_allocator,
                                                 ub_data->allocation,
                                                 0,
                                                 ub.data_size()));
            }
            ub_data->version = ub.version();
        }

        auto desc_key = std::make_tuple(&ub,
                                        std::vector<mge::texture*>{},
                                        vk_program.descriptor_set_layout());
        VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
        auto            desc_it = m_descriptor_sets.find(desc_key);

        if (desc_it != m_descriptor_sets.end()) {
            descriptor_set = desc_it->second;
        } else {
            VkDescriptorSetLayout layout = vk_program.descriptor_set_layout();
            if (layout == VK_NULL_HANDLE) {
                return VK_NULL_HANDLE;
            }

            VkDescriptorSetAllocateInfo alloc_info = {};
            alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            alloc_info.descriptorPool = m_descriptor_pool;
            alloc_info.descriptorSetCount = 1;
            alloc_info.pSetLayouts = &layout;

            VkResult result = vkAllocateDescriptorSets(m_device,
                                                       &alloc_info,
                                                       &descriptor_set);
            if (result != VK_SUCCESS) {
                return VK_NULL_HANDLE;
            }

            VkDescriptorBufferInfo buffer_info = {};
            buffer_info.buffer = ub_data->buffer;
            buffer_info.offset = 0;
            buffer_info.range = ub.data_size();

            uint32_t binding_point = 0;
            for (const auto& ub_metadata : vk_program.uniform_buffers()) {
                if (ub_metadata.name == ub.name()) {
                    binding_point = ub_metadata.location;
                    break;
                }
            }

            VkWriteDescriptorSet descriptor_write = {};
            descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write.dstSet = descriptor_set;
            descriptor_write.dstBinding = binding_point;
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptor_write.descriptorCount = 1;
            descriptor_write.pBufferInfo = &buffer_info;

            vkUpdateDescriptorSets(m_device, 1, &descriptor_write, 0, nullptr);

            m_descriptor_sets[desc_key] = descriptor_set;
        }

        return descriptor_set;
    }

    void render_context_base::bind_texture(VkCommandBuffer       command_buffer,
                                           mge::vulkan::program& vk_program,
                                           mge::texture*         tex,
                                           mge::uniform_block*   ub)
    {
        mge::texture_binding_list textures;
        if (tex) {
            textures.push_back({0, tex});
        }
        VkDescriptorSet descriptor_set =
            prepare_texture(vk_program, textures, ub);
        if (descriptor_set != VK_NULL_HANDLE) {
            vkCmdBindDescriptorSets(command_buffer,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    vk_program.pipeline_layout(),
                                    0,
                                    1,
                                    &descriptor_set,
                                    0,
                                    nullptr);
        }
    }

    VkDescriptorSet render_context_base::prepare_texture(
        mge::vulkan::program&            vk_program,
        const mge::texture_binding_list& textures,
        mge::uniform_block*              ub)
    {
        if (textures.empty()) {
            return VK_NULL_HANDLE;
        }

        const auto& sampler_bindings = vk_program.sampler_bindings();
        if (sampler_bindings.empty()) {
            return VK_NULL_HANDLE;
        }

        VkDescriptorSetLayout layout = vk_program.descriptor_set_layout();
        if (layout == VK_NULL_HANDLE) {
            return VK_NULL_HANDLE;
        }

        std::vector<mge::texture*> tex_ptrs;
        tex_ptrs.reserve(textures.size());
        for (const auto& b : textures) {
            tex_ptrs.push_back(b.texture);
        }

        auto desc_key =
            std::make_tuple(ub ? ub : static_cast<mge::uniform_block*>(nullptr),
                            tex_ptrs,
                            layout);

        VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
        auto            desc_it = m_descriptor_sets.find(desc_key);

        if (desc_it != m_descriptor_sets.end()) {
            descriptor_set = desc_it->second;
        } else {
            auto ub_only_key = std::make_tuple(
                ub ? ub : static_cast<mge::uniform_block*>(nullptr),
                std::vector<mge::texture*>{},
                layout);
            auto ub_it = m_descriptor_sets.find(ub_only_key);

            if (ub && ub_it != m_descriptor_sets.end()) {
                VkDescriptorSetAllocateInfo alloc_info = {};
                alloc_info.sType =
                    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                alloc_info.descriptorPool = m_descriptor_pool;
                alloc_info.descriptorSetCount = 1;
                alloc_info.pSetLayouts = &layout;

                VkResult result = vkAllocateDescriptorSets(m_device,
                                                           &alloc_info,
                                                           &descriptor_set);
                if (result != VK_SUCCESS) {
                    return VK_NULL_HANDLE;
                }

                auto ub_buf_it = m_uniform_buffers.find(ub);
                if (ub_buf_it != m_uniform_buffers.end()) {
                    VkDescriptorBufferInfo buffer_info = {};
                    buffer_info.buffer = ub_buf_it->second.buffer;
                    buffer_info.offset = 0;
                    buffer_info.range = ub->data_size();

                    uint32_t binding_point = 0;
                    for (const auto& ubm : vk_program.uniform_buffers()) {
                        if (ubm.name == ub->name()) {
                            binding_point = ubm.location;
                            break;
                        }
                    }

                    VkWriteDescriptorSet ubo_write = {};
                    ubo_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ubo_write.dstSet = descriptor_set;
                    ubo_write.dstBinding = binding_point;
                    ubo_write.dstArrayElement = 0;
                    ubo_write.descriptorType =
                        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ubo_write.descriptorCount = 1;
                    ubo_write.pBufferInfo = &buffer_info;

                    vkUpdateDescriptorSets(m_device, 1, &ubo_write, 0, nullptr);
                }
            } else {
                VkDescriptorSetAllocateInfo alloc_info = {};
                alloc_info.sType =
                    VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                alloc_info.descriptorPool = m_descriptor_pool;
                alloc_info.descriptorSetCount = 1;
                alloc_info.pSetLayouts = &layout;

                VkResult result = vkAllocateDescriptorSets(m_device,
                                                           &alloc_info,
                                                           &descriptor_set);
                if (result != VK_SUCCESS) {
                    return VK_NULL_HANDLE;
                }
            }

            for (const auto& sb : sampler_bindings) {
                mge::texture* matched_tex = nullptr;
                for (const auto& binding : textures) {
                    if (binding.slot == sb.binding) {
                        matched_tex = binding.texture;
                        break;
                    }
                }
                if (!matched_tex) {
                    continue;
                }

                auto* vk_tex = static_cast<mge::vulkan::texture*>(matched_tex);

                VkDescriptorImageInfo image_info = {};
                image_info.imageLayout =
                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                image_info.imageView = vk_tex->image_view();
                image_info.sampler = vk_tex->sampler();

                VkWriteDescriptorSet descriptor_write = {};
                descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_write.dstSet = descriptor_set;
                descriptor_write.dstBinding = sb.binding;
                descriptor_write.dstArrayElement = 0;
                descriptor_write.descriptorType =
                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptor_write.descriptorCount = 1;
                descriptor_write.pImageInfo = &image_info;

                vkUpdateDescriptorSets(m_device, 1, &descriptor_write, 0,
                                       nullptr);
            }

            m_descriptor_sets[desc_key] = descriptor_set;
        }

        return descriptor_set;
    }

    void render_context_base::draw_geometry(
        VkCommandBuffer                  command_buffer,
        mge::program*                    prog,
        mge::vertex_buffer*              vb,
        mge::index_buffer*               ib,
        const mge::pipeline_state&       state,
        mge::uniform_block*              ub,
        const mge::texture_binding_list& textures,
        VkRenderPass                     render_pass,
        uint32_t                         index_count,
        uint32_t                         index_offset)
    {
        mge::vulkan::program* vk_program =
            static_cast<mge::vulkan::program*>(prog);
        mge::vulkan::vertex_buffer* vk_vertex_buffer =
            static_cast<mge::vulkan::vertex_buffer*>(vb);
        mge::vulkan::index_buffer* vk_index_buffer =
            static_cast<mge::vulkan::index_buffer*>(ib);

        VkPipeline p =
            this->pipeline(*vk_vertex_buffer, *vk_program, state, render_pass);
        vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p);

        if (ub || !textures.empty()) {
            VkDescriptorSet descriptor_set = VK_NULL_HANDLE;
            if (ub) {
                descriptor_set = prepare_uniform_block(*vk_program, *ub);
            }
            if (!textures.empty()) {
                descriptor_set = prepare_texture(*vk_program, textures, ub);
            }
            if (descriptor_set != VK_NULL_HANDLE) {
                vkCmdBindDescriptorSets(command_buffer,
                                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                                        vk_program->pipeline_layout(),
                                        0,
                                        1,
                                        &descriptor_set,
                                        0,
                                        nullptr);
            }
        }

        VkDeviceSize offsets[1]{0};
        VkBuffer     buffers[1]{vk_vertex_buffer->vk_buffer()};
        vkCmdBindVertexBuffers(command_buffer, 0, 1, buffers, offsets);
        vkCmdBindIndexBuffer(command_buffer,
                             vk_index_buffer->vk_buffer(),
                             0,
                             vk_index_buffer->vk_index_type());
        uint32_t count =
            index_count > 0
                ? index_count
                : static_cast<uint32_t>(vk_index_buffer->element_count());
        vkCmdDrawIndexed(command_buffer, count, 1, index_offset, 0, 1);
    }

    const std::vector<VkVertexInputAttributeDescription>&
    render_context_base::vertex_input_attribute_descriptions(
        const mge::vertex_layout& layout)
    {
        auto it = m_vertex_input_attribute_descriptions.find(layout);
        if (it != m_vertex_input_attribute_descriptions.end()) {
            return it->second;
        }
        std::vector<VkVertexInputAttributeDescription> descriptions;
        uint32_t                                       offset = 0;
        uint32_t                                       location = 0;
        for (const auto& el : layout) {
            VkVertexInputAttributeDescription desc;
            desc.binding = 0;
            desc.location = location++;
            desc.format = vk_format(el.format);
            desc.offset = offset;
            descriptions.emplace_back(desc);
            offset += static_cast<uint32_t>(el.format.binary_size());
        }
        return m_vertex_input_attribute_descriptions[layout] =
                   std::move(descriptions);
    }

    VkPipeline render_context_base::pipeline(const vertex_buffer&       buffer,
                                             const program&             prog,
                                             const mge::pipeline_state& state,
                                             VkRenderPass               render_pass)
    {
        pipeline_key_type key{buffer.vk_buffer(),
                              prog.pipeline_layout(),
                              state,
                              render_pass};

        auto it = m_pipelines.find(key);
        if (it != m_pipelines.end()) {
            return it->second;
        }

        VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                           VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {};
        dynamic_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = std::size(dynamic_states);
        dynamic_state_create_info.pDynamicStates = dynamic_states;

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info =
            {};
        vertex_input_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
        vertex_input_state_create_info.pVertexBindingDescriptions =
            &buffer.binding_description();
        vertex_input_state_create_info.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(buffer.attribute_descriptions().size());
        vertex_input_state_create_info.pVertexAttributeDescriptions =
            buffer.attribute_descriptions().data();

        VkPipelineInputAssemblyStateCreateInfo
            input_assembly_state_create_info = {};
        input_assembly_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_state_create_info.topology =
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        viewport_state_create_info.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info =
            {};
        rasterization_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state_create_info.depthClampEnable = VK_FALSE;
        rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization_state_create_info.lineWidth = 1.0f;

        mge::cull_mode cull = state.cull_mode();
        switch (cull) {
        case mge::cull_mode::NONE:
            rasterization_state_create_info.cullMode = VK_CULL_MODE_NONE;
            break;
        case mge::cull_mode::CLOCKWISE:
            rasterization_state_create_info.cullMode = VK_CULL_MODE_FRONT_BIT;
            break;
        case mge::cull_mode::COUNTER_CLOCKWISE:
            rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
            break;
        }
        rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterization_state_create_info.depthBiasEnable = VK_FALSE;
        rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
        rasterization_state_create_info.depthBiasClamp = 0.0f;
        rasterization_state_create_info.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampling_create_info{};
        multisampling_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling_create_info.sampleShadingEnable = VK_FALSE;
        multisampling_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling_create_info.minSampleShading = 1.0f;
        multisampling_create_info.pSampleMask = nullptr;
        multisampling_create_info.alphaToCoverageEnable = VK_FALSE;
        multisampling_create_info.alphaToOneEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info =
            {};
        depth_stencil_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil_state_create_info.depthTestEnable = VK_TRUE;
        depth_stencil_state_create_info.depthWriteEnable =
            state.depth_write() ? VK_TRUE : VK_FALSE;
        depth_stencil_state_create_info.depthCompareOp =
            depth_test_to_vulkan(state.depth_test_function());
        depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE;
        depth_stencil_state_create_info.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
        color_blend_attachment_state.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        auto color_blend_operation = state.color_blend_operation();

        if (color_blend_operation == mge::blend_operation::NONE) {
            color_blend_attachment_state.blendEnable = VK_FALSE;
            color_blend_attachment_state.srcColorBlendFactor =
                VK_BLEND_FACTOR_ONE;
            color_blend_attachment_state.dstColorBlendFactor =
                VK_BLEND_FACTOR_ZERO;
            color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
            color_blend_attachment_state.srcAlphaBlendFactor =
                VK_BLEND_FACTOR_ONE;
            color_blend_attachment_state.dstAlphaBlendFactor =
                VK_BLEND_FACTOR_ZERO;
            color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;
        } else {
            auto color_src_factor = state.color_blend_factor_src();
            auto color_dst_factor = state.color_blend_factor_dst();
            auto alpha_blend_operation = state.alpha_blend_operation();
            auto alpha_src_factor = state.alpha_blend_factor_src();
            auto alpha_dst_factor = state.alpha_blend_factor_dst();

            color_blend_attachment_state.blendEnable = VK_TRUE;
            color_blend_attachment_state.srcColorBlendFactor =
                blend_factor_to_vulkan(color_src_factor);
            color_blend_attachment_state.dstColorBlendFactor =
                blend_factor_to_vulkan(color_dst_factor);
            color_blend_attachment_state.colorBlendOp =
                blend_operation_to_vulkan(color_blend_operation);
            color_blend_attachment_state.srcAlphaBlendFactor =
                blend_factor_to_vulkan(alpha_src_factor);
            color_blend_attachment_state.dstAlphaBlendFactor =
                blend_factor_to_vulkan(alpha_dst_factor);
            color_blend_attachment_state.alphaBlendOp =
                blend_operation_to_vulkan(alpha_blend_operation);
        }

        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {};
        color_blend_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_create_info.logicOpEnable = VK_FALSE;
        color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_create_info.attachmentCount = 1;
        color_blend_state_create_info.pAttachments =
            &color_blend_attachment_state;
        color_blend_state_create_info.blendConstants[0] = 0.0f;
        color_blend_state_create_info.blendConstants[1] = 0.0f;
        color_blend_state_create_info.blendConstants[2] = 0.0f;
        color_blend_state_create_info.blendConstants[3] = 0.0f;

        VkPipelineLayout pipeline_layout{prog.pipeline_layout()};

        VkGraphicsPipelineCreateInfo pipeline_create_info = {};
        pipeline_create_info.sType =
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_create_info.stageCount =
            static_cast<uint32_t>(prog.shader_stage_create_infos().size());
        pipeline_create_info.pStages = prog.shader_stage_create_infos().data();
        pipeline_create_info.pVertexInputState =
            &vertex_input_state_create_info;
        pipeline_create_info.pInputAssemblyState =
            &input_assembly_state_create_info;
        pipeline_create_info.pViewportState = &viewport_state_create_info;
        pipeline_create_info.pRasterizationState =
            &rasterization_state_create_info;
        pipeline_create_info.pMultisampleState = &multisampling_create_info;
        pipeline_create_info.pDepthStencilState =
            &depth_stencil_state_create_info;
        pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
        pipeline_create_info.pDynamicState = &dynamic_state_create_info;
        pipeline_create_info.layout = pipeline_layout;
        pipeline_create_info.renderPass = render_pass;
        pipeline_create_info.subpass = 0;
        pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_create_info.basePipelineIndex = -1;

        VkPipeline new_pipeline{VK_NULL_HANDLE};
        CHECK_VK_CALL(vkCreateGraphicsPipelines(device(),
                                                VK_NULL_HANDLE,
                                                1,
                                                &pipeline_create_info,
                                                nullptr,
                                                &new_pipeline));
        return m_pipelines[key] = new_pipeline;
    }

} // namespace mge::vulkan
