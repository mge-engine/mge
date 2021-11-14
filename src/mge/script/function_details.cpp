#include "mge/script/function_details.hpp"
#include "mge/script/module_details.hpp"
#include "mge/script/type_details.hpp"

namespace mge::script {
    function_details::function_details(const std::string& name)
        : m_name(name)
    {}

    const std::string& function_details::name() const { return m_name; }

    void function_details::apply(visitor& v)
    {
        v.begin(*this);
        v.end(*this);
    }

} // namespace mge::script