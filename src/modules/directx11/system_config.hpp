#pragma once
#include "mge/core/configuration.hpp"

namespace dx11 {
    class system_config
    {
    public:
        system_config()
            :m_debug(false),
             m_software_device(false)
        {}

        void configure(const mge::configuration& config)
        {
            m_debug = config.value<bool>("debug", false);
            m_software_device = config.value<bool>("software_device", false);
        }

        bool debug() const
        {
            return m_debug;
        }

        bool software_device() const
        {
            return m_software_device;
        }
    private:
        bool m_debug;
        bool m_software_device;
    };
}
