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

    /**
     * Base class for closures. A closure is a function object that exposes
     * a function pointer to be called from C code.
     *
     * To create a closure, derive from this class and implement the
     * <tt>execute</tt> method.
     *
     * Note that the closure object must be kept alive as long as the
     * function pointer is used.
     */
    template <typename R, typename... Args> class closure : public closure_base
    {
    public:
        /**
         * Type of the function pointer.
         */
        using function_type = R (*)(Args...);

        /**
         * Constructor.
         */
        closure()
            : m_arg_types{{(mge::ffi_compute_type<Args>::type())...}}
        {
            prepare(mge::ffi_compute_type<R>::type(),
                    m_arg_types.data(),
                    sizeof...(Args),
                    &binding_function,
                    this);
        }

        /**
         * Destructor.
         */
        ~closure() = default;

        /**
         * Execute the closure.
         */
        virtual R execute(Args...) = 0;

        /**
         * Returns the function pointer.
         * @return function pointer
         */
        function_type function() const
        {
            return reinterpret_cast<function_type>(m_executable_address);
        }

    private:
        static void
        binding_function(ffi_cif* cif, void* ret, void* args[], void* userdata)
        {
            auto self = reinterpret_cast<closure*>(userdata);
            R*   ret_address = reinterpret_cast<R*>(ret);
            *ret_address =
                self->execute((*reinterpret_cast<Args*>(args[0]))...);
        }

        std::array<ffi_type*, sizeof...(Args)> m_arg_types;
    };

    template <typename... Args>
    class closure<void, Args...> : public closure_base
    {
    public:
        /**
         * Type of the function pointer.
         */
        using function_type = void (*)(Args...);

        /**
         * Constructor.
         */
        closure()
            : m_arg_types{{mge::ffi_compute_type<Args>::type()...}}
        {
            prepare(mge::ffi_compute_type<void>::type(),
                    m_arg_types.data(),
                    sizeof...(Args),
                    &binding_function,
                    this);
        }

        /**
         * Destructor.
         */
        ~closure() = default;

        /**
         * Execute the closure.
         */
        virtual void execute(Args...) = 0;

        /**
         * Returns the function pointer.
         * @return function pointer
         */
        function_type function() const
        {
            return reinterpret_cast<function_type>(m_executable_address);
        }

    private:
        static void
        binding_function(ffi_cif* cif, void* ret, void* args[], void* userdata)
        {
            auto self = reinterpret_cast<closure*>(userdata);
            self->execute((*reinterpret_cast<Args*>(args[0]))...);
        }

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

        function_type function() const
        {
            return reinterpret_cast<function_type>(m_executable_address);
        }

    private:
        static void
        binding_function(ffi_cif* cif, void* ret, void* args[], void* userdata)
        {
            auto self = reinterpret_cast<closure*>(userdata);
            self->execute();
        }
    };

} // namespace mge