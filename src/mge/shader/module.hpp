#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/shader/shader_fwd.hpp"
#include "mge/shader/program_element.hpp"
#include <string_view>

namespace mge {
namespace shader {

    /**
     * @brief A source code text, or file.
     */
    class MGE_SHADER_EXPORT module : public program_element
    {
    public:
        module(mge::shader::program *p,
               const std::string& name,
               const std::string& text);
        virtual ~module() = default;
        std::string_view name() const;
        std::string_view text() const;
    private:
        mge::shader::string m_name;
        mge::shader::string m_text;
    };

}
}
