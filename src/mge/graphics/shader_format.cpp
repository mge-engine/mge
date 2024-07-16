// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_format.hpp"

namespace mge {

    shader_format::shader_format(std::string_view name)
        : m_name(name)
    {}

    shader_format::shader_format(std::string_view        name,
                                 const semantic_version& version)
        : m_name(name)
        , m_version(version)
    {}

    bool shader_format::operator==(const shader_format& other) const
    {
        return m_name == other.m_name && m_version == other.m_version;
    }

    bool shader_format::operator!=(const shader_format& other) const
    {
        return m_name != other.m_name || m_version != other.m_version;
    }

} // namespace mge