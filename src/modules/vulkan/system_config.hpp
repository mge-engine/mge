#pragma once
#include "mge/core/configuration.hpp"
#include "mge/core/log.hpp"

namespace vulkan {

    class system_config
    {
    public:
        system_config()
            :m_debug(false)
        {}
        
        void configure(const mge::configuration& config)
        {
            m_debug = config.value<bool>("debug", false);
            if (config.contains_key("layers")) {
                m_layers = config.list_value("layers");
            }
        }

        bool debug() const
        {
            return m_debug;
        }

        const std::vector<std::string>& layers() const
        {
            return m_layers;
        }
    private:
        bool m_debug;
        std::vector<std::string> m_layers;
    };

}