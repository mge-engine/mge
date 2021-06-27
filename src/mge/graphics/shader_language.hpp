#pragma once
#include "mge/core/semantic_version.hpp"
#include "mge/graphics/dllexport.hpp"

#include <string>
#include <string_view>

namespace mge {
    class MGEGRAPHICS_EXPORT shader_language
    {
    public:
        shader_language(std::string_view name, const semantic_version &version);
        ~shader_language();

    private:
        std::string      m_name;
        semantic_version m_version;
    };

} // namespace mge