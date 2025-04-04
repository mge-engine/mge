#include "mge/script/proxy.hpp"
#include "mge/script/invocation_context.hpp"

#include "mge/core/trace.hpp"
namespace mge {
    MGE_USE_TRACE(SCRIPT);
}

namespace mge::script {

    proxy_base::proxy_base()
        : m_context(nullptr)
    {
        MGE_DEBUG_TRACE(SCRIPT) << "proxy@" << this << " ctor";
    }

    proxy_base::~proxy_base()
    {
        MGE_DEBUG_TRACE(SCRIPT) << "proxy@" << this << " dtor";
        delete m_context;
    }

    void proxy_base::set_context(invocation_context* context)
    {
        MGE_DEBUG_TRACE(SCRIPT)
            << "proxy@" << this << " set context@" << context;
        m_context = context;
    }

    void proxy_base::clear_context()
    {
        MGE_DEBUG_TRACE(SCRIPT) << "proxy@" << this << " clear context";
        m_context = nullptr;
    }

} // namespace mge::script