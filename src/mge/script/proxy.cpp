#include "mge/script/proxy.hpp  "
#include "mge/core/trace.hpp"
namespace mge {
    MGE_USE_TRACE(SCRIPT);
}

namespace mge::script {

    proxy_base::proxy_base()
    {
        MGE_DEBUG_TRACE(SCRIPT) << "proxy@" << this << " ctor";
    }

    proxy_base::~proxy_base()
    {
        MGE_DEBUG_TRACE(SCRIPT) << "proxy@" << this << " dtor";
    }

    void proxy_base::set_context(invocation_context* context)
    {
        MGE_DEBUG_TRACE(SCRIPT)
            << "proxy@" << this << " set context@" << context;
        m_context = context;
    }

} // namespace mge::script