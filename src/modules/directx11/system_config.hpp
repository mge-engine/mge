// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/configuration.hpp"

namespace dx11 {
    /**
     * DirectX 11 system config.
     */
    class system_config
    {
    public:
        system_config()
            :m_debug(false),
             m_software_device(false)
        {}

        /**
         * Evaluate configuration.
         * 
         * @param config evaluated configuration
         */
        void configure(const mge::configuration& config)
        {
            m_debug = config.value<bool>("debug", false);
            m_software_device = config.value<bool>("software_device", false);
        }

        /**
         * Return whether debugging shall be enabled.
         * 
         * @return @c true if debugging is enabled
         */
        bool debug() const noexcept
        {
            return m_debug;
        }

        /**
         * Return whether a software device is used.
         * 
         * @return @c true if a software device is used
         */
        bool software_device() const noexcept
        {
            return m_software_device;
        }
    private:
        bool m_debug;
        bool m_software_device;
    };
}
