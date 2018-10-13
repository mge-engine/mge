/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "mge/graphics/monitor.hpp"

namespace mge {
    monitor::monitor(const std::string& name,
                     bool primary)
    :m_name(name),
     m_primary(primary)
    {}

    const std::string&
    monitor::name() const
    {
        return m_name;
    }

    bool
    monitor::primary() const
    {
        return m_primary;
    }
}
