#include "mge/script/type.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type_details.hpp"

namespace mge::script {
    const std::string& type_base::name() const { return m_details->name(); }

    mge::script::module type_base::module() const
    {
        return mge::script::module(m_details->module());
    }

    const std::type_index& type_base::type_index() const
    {
        return m_details->type_index();
    }

    type_details* type_base::details() const { return m_details; }

    void type_base::init_details(const std::type_index& ti)
    {
        m_details = type_details::get(ti);
    }

} // namespace mge::script