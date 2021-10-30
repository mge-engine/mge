#include "mge/script/script_module.hpp"
#include "mge/core/unprotect_constructor.hpp"

namespace mge::script {
    module::module(std::string_view name)
        : m_name(name)
    {}

    module::~module() {}

    module_ref module::parent() const
    {
        std::shared_ptr<module> res(m_parent);
        return res;
    }

    std::string_view module::name() const { return m_name; }

    std::string module::full_name() const
    {
        if (is_root()) {
            return "";
        } else {
            return parent()->full_name() + "::" + m_name;
        }
    }

    bool module::is_root() const { return m_name.empty(); }

    module_ref module::root()
    {

        static module_ref s_root =
            std::make_shared<unprotect_constructor<module>>("");
        return s_root;
    }

} // namespace mge::script