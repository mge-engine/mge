#include "mge/graphics/shader_format.hpp"

namespace mge {

    shader_format::shader_format() {}

    shader_format::shader_format(std::string_view name)
        : m_name(name)
    {}

    shader_format::shader_format(std::string_view        name,
                                 const semantic_version& version)
        : m_name(name)
        , m_version(version)
    {}

    shader_format::~shader_format() {}
} // namespace mge