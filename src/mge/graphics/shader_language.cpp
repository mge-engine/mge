#include "mge/graphics/shader_language.hpp"

namespace mge {
    shader_language::shader_language(std::string_view        name,
                                     const semantic_version &version)
        : m_name(name), m_version(version)
    {}

    shader_language::~shader_language() {}
} // namespace mge