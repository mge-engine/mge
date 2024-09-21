#pragma once
#include "mge/core/dllexport.hpp"
#include <ffi.h>

#include <array>
#include <cstdint>
#include <type_traits>

namespace mge {
    class MGECORE_EXPORT closure_base
    {
    public:
        closure_base();
        virtual ~closure_base();

    protected:
        void prepare(ffi_type*  return_type,
                     ffi_type** arg_types,
                     uint32_t   nargs,
                     void*      binding_function,
                     void*      user_data);

        void*   m_writable_address{0};
        void*   m_executable_address{0};
        ffi_cif m_cif;
    };

    template <typename T> struct ffi_compute_type;

    template <> struct ffi_compute_type<uint8_t>
    {
        static ffi_type* type() { return &ffi_type_uint8; }
    };
    template <> struct ffi_compute_type<int8_t>
    {
        static ffi_type* type() { return &ffi_type_sint8; }
    };

    template <> struct ffi_compute_type<uint16_t>
    {
        static ffi_type* type() { return &ffi_type_uint16; }
    };

    template <> struct ffi_compute_type<int16_t>
    {
        static ffi_type* type() { return &ffi_type_sint16; }
    };
    template <> struct ffi_compute_type<uint32_t>
    {
        static ffi_type* type() { return &ffi_type_uint32; }
    };
    template <> struct ffi_compute_type<int32_t>
    {
        static ffi_type* type() { return &ffi_type_sint32; }
    };

    template <> struct ffi_compute_type<uint64_t>
    {
        static ffi_type* type() { return &ffi_type_uint64; }
    };

    template <> struct ffi_compute_type<int64_t>
    {
        static ffi_type* type() { return &ffi_type_sint64; }
    };

    template <> struct ffi_compute_type<float>
    {
        static ffi_type* type() { return &ffi_type_float; }
    };

    template <> struct ffi_compute_type<double>
    {
        static ffi_type* type() { return &ffi_type_double; }
    };

    template <> struct ffi_compute_type<long double>
    {
        static ffi_type* type() { return &ffi_type_longdouble; }
    };

    template <> struct ffi_compute_type<void>
    {
        static ffi_type* type() { return &ffi_type_void; }
    };

    template <> struct ffi_compute_type<bool>
    {
        static ffi_type* type() { return &ffi_type_uint8; }
    };

    template <typename T> struct ffi_compute_type<T*>
    {
        static ffi_type* type() { return &ffi_type_pointer; }
    };
    template <typename T> struct ffi_compute_type<const T*>
    {
        static ffi_type* type() { return &ffi_type_pointer; }
    };

    template <typename R, typename... Args> class closure : public closure_base
    {
    public:
        using function_type = R (*)(Args...);

        closure()
        {
            m_arg_types = {{mge::ffi_compute_type<Args>::type()}...};
            prepare(mge::ffi_compute_type<R>::type(),
                    m_arg_types.data(),
                    sizeof...(Args),
                    &binding_function,
                    this);
        }

        ~closure() = default;

        virtual R execute(Args...) = 0;

        static void
        binding_function(ffi_cif* cif, void* ret, void* args[], void* userdata)
        {
            auto self = reinterpret_cast<closure*>(userdata);
            R*   ret_address = reinterpret_cast<R*>(ret);
            *ret_address =
                self->execute((*reinterpret_cast<Args*>(args[0]))...);
        }

        function_type function() const
        {
            return reinterpret_cast<function_type>(m_executable_address);
        }

    private:
        std::array<ffi_type*, sizeof...(Args)> m_arg_types;
    };

    template <> class closure<void, void> : public closure_base
    {
    public:
        using function_type = void (*)();

        closure()
        {
            prepare(mge::ffi_compute_type<void>::type(),
                    nullptr,
                    0,
                    &binding_function,
                    this);
        }

        ~closure() = default;

        virtual void execute() = 0;

        static void
        binding_function(ffi_cif* cif, void* ret, void* args[], void* userdata)
        {
            auto self = reinterpret_cast<closure*>(userdata);
            self->execute();
        }

        function_type function() const
        {
            return reinterpret_cast<function_type>(m_executable_address);
        }
    };

} // namespace mge