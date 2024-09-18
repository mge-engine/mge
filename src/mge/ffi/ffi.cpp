#include "ffi.hpp"

namespace mge {

    closure::closure()
    {
        m_writable_address =
            ffi_closure_alloc((sizeof(ffi_closure)), &m_executable_address);
    }

    closure::~closure()
    {
        if (m_writable_address) {
            ffi_closure_free(m_writable_address);
            m_writable_address = nullptr;
        }
        m_executable_address = nullptr;
    }
} // namespace mge