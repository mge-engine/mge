#include "mge/shader/module.hpp"
#include "mge/shader/program.hpp"
namespace mge {
namespace shader {
    module::module(mge::shader::program *p,
                   const std::string& name,
                   const std::string& text)
        : program_element(p)
        , m_name(name.begin(), name.end())
        , m_text(text.begin(), text.end())
    {}

    std::string_view
    module::name() const
    {
        return std::string_view(m_name.c_str(), m_name.size());
    }

    std::string_view
    module::text() const
    {
        return std::string_view(m_text.c_str(), m_text.size());
    }

}
}
