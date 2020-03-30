// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/configuration.hpp"
#include "mge/core/log.hpp"

namespace vulkan {

    class system_config
    {
    public:
        system_config()
            : m_debug(false), m_validation(false), m_configured(false)
        {}

        void configure(const mge::configuration &config)
        {
            m_debug      = config.value<bool>("debug", false);
            m_validation = config.value<bool>("validation", false);
            if (config.contains_key("layers")) {
                m_layers = config.list_value("layers");
            }
            for (const auto &l : m_layers) {
                m_layer_names.push_back(l.c_str());
            }
            m_configured = true;
        }

        bool debug() const { return m_debug; }

        bool validation() const noexcept { return m_validation; }

        bool configured() const noexcept { return m_configured; }

        const std::vector<const char *> &layers() const
        {
            return m_layer_names;
        }

    private:
        bool                      m_debug;
        bool                      m_validation;
        bool                      m_configured;
        std::vector<std::string>  m_layers;
        std::vector<const char *> m_layer_names;
    };

} // namespace vulkan