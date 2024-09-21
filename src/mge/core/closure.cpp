#include "closure.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    closure_base::closure_base()
    {
        m_writable_address =
            ffi_closure_alloc((sizeof(ffi_closure)), &m_executable_address);
    }

    closure_base::~closure_base()
    {
        if (m_writable_address) {
            ffi_closure_free(m_writable_address);
            m_writable_address = nullptr;
        }
        m_executable_address = nullptr;
    }

    void closure_base::prepare(ffi_type*  return_type,
                               ffi_type** arg_types,
                               uint32_t   nargs,
                               void*      binding_function,
                               void*      user_data)
    {

        ffi_status status = ffi_prep_cif(&m_cif,
                                         FFI_DEFAULT_ABI,
                                         nargs,
                                         return_type,
                                         arg_types);
        if (status != FFI_OK) {
            MGE_THROW(mge::runtime_exception) << "ffi_prep_cif failed";
        }
        status = ffi_prep_closure_loc(
            reinterpret_cast<ffi_closure*>(m_writable_address),
            &m_cif,
            reinterpret_cast<void (*)(ffi_cif*, void*, void**, void*)>(
                binding_function),
            user_data,
            m_executable_address);
        if (status != FFI_OK) {
            MGE_THROW(mge::runtime_exception) << "ffi_prep_closure_loc failed";
        }
    }
} // namespace mge