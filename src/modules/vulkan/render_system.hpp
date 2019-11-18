#pragma once
#include "vulkan.hpp"
#include "system_config.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/core/small_vector.hpp"
#include "vulkan_library.hpp"
#ifdef MGE_OS_WINDOWS
#  include "win32/monitor.hpp"
#endif

namespace vulkan {


    class render_system : public mge::render_system
    {
    public:
        using extension_property_vector = std::vector<VkExtensionProperties>;
        using layer_property_vector = std::vector<VkLayerProperties>;
        using physical_device_vector = mge::small_vector<VkPhysicalDevice, 3>;
        using queue_family_property_vector = mge::small_vector<VkQueueFamilyProperties, 4>;

        render_system();
        virtual ~render_system();

        void configure(const mge::configuration& config) override;

        monitor_collection_t monitors() const override;
        mge::monitor_ref primary_monitor() const override;
        mge::window_ref create_window(const mge::rectangle& rect,
                                      const mge::window_options& options) override;

        VkInstance instance() const noexcept
        {
            return m_instance;
        }

        VkPhysicalDevice physical_device() const noexcept
        {
            return m_physical_device;
        }

        VkDevice device() const noexcept
        {
            return m_device;
        }

        const queue_family_property_vector& queue_family_properties() const
        {
            return m_physical_device_queue_family_properties;
        }


#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X;
#define INSTANCE_FUNCTION(X)       PFN_##X X;
#define DEVICE_FUNCTION(X)         PFN_##X X;

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#  include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

    private:

        template <typename F, typename C>
        void fill_enumeration(const F& function, C& container);

        void check_configure();
        void scan_properties();
        void init_instance_extensions();
        void create_instance();
        void resolve_basic_instance_functions();
        void resolve_normal_instance_functions();
        void init_debug_message_handling();
        void load_physical_devices();
        void compute_queue_family_indices();
        uint32_t best_queue_family_index(VkQueueFlagBits);
        void create_device();
        void resolve_device_functions();
        void retrieve_device_queues();


        system_config                    m_config;
        std::shared_ptr<vulkan_library>  m_library;

        extension_property_vector        m_all_instance_extensions;
        layer_property_vector            m_all_instance_layers;

        physical_device_vector           m_physical_devices;
        uint32_t                         m_selected_physical_device_index;
        VkPhysicalDevice                 m_physical_device;

        VkPhysicalDeviceProperties       m_physical_device_properties;
        VkPhysicalDeviceFeatures         m_physical_device_features;
        VkPhysicalDeviceMemoryProperties m_physical_device_memory_properties;
        queue_family_property_vector     m_physical_device_queue_family_properties;
        extension_property_vector        m_physical_device_extensions;


        std::vector<const char*>         m_instance_extensions;
        VkInstance                       m_instance;
        VkDebugUtilsMessengerEXT         m_debug_messenger;

        struct
        {
            uint32_t graphics;
            uint32_t compute;
            uint32_t transfer;
        }                               m_queue_family_indices;


        std::vector<const char*>         m_enabled_device_extensions;
        VkDevice                         m_device;

        VkQueue                          m_graphics_queue;
        VkQueue                          m_compute_queue;
        VkQueue                          m_transfer_queue;

    };
}
