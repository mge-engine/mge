#include "mge/script/type.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type_details.hpp"

namespace mge::script {
    const std::string& type_base::name() const { return m_details->name(); }

    mge::script::module type_base::module() const
    {
        return mge::script::module(m_details->module());
    }

} // namespace mge::script