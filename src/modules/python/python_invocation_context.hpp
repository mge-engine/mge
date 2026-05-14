// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/reflection/invocation_context.hpp"

#include "pyobject_ref.hpp"
#include "python.hpp"

#include <string>
#include <vector>

namespace mge::python {

    /**
     * @brief Invocation context for calling Python methods from C++.
     *
     * Attached to a proxy object after construction. When the proxy's
     * C++ virtual methods are called, they delegate here to dispatch
     * into the Python override on the Python-side instance.
     */
    class python_invocation_context : public mge::reflection::invocation_context
    {
    public:
        explicit python_invocation_context(PyObject* self);
        ~python_invocation_context() override;

        bool call_implemented(const char* method) override;

        PyObject* self() const { return m_self.get(); }

    protected:
        void store_bool_argument(size_t index, bool value) override;
        void store_int8_t_argument(size_t index, int8_t value) override;
        void store_uint8_t_argument(size_t index, uint8_t value) override;
        void store_int16_t_argument(size_t index, int16_t value) override;
        void store_uint16_t_argument(size_t index, uint16_t value) override;
        void store_int32_t_argument(size_t index, int32_t value) override;
        void store_uint32_t_argument(size_t index, uint32_t value) override;
        void store_int64_t_argument(size_t index, int64_t value) override;
        void store_uint64_t_argument(size_t index, uint64_t value) override;
        void store_float_argument(size_t index, float value) override;
        void store_double_argument(size_t index, double value) override;
        void store_long_double_argument(size_t index, long double value) override;
        void store_string_argument(size_t             index,
                                   const std::string& value) override;

        call_result_type call_method(const char* method) override;

        bool        get_bool_result() override;
        int8_t      get_int8_t_result() override;
        uint8_t     get_uint8_t_result() override;
        int16_t     get_int16_t_result() override;
        uint16_t    get_uint16_t_result() override;
        int32_t     get_int32_t_result() override;
        uint32_t    get_uint32_t_result() override;
        int64_t     get_int64_t_result() override;
        uint64_t    get_uint64_t_result() override;
        float       get_float_result() override;
        double      get_double_result() override;
        long double get_long_double_result() override;
        std::string get_string_result() override;

    private:
        void set_arg(size_t index, PyObject* obj);

        pyobject_ref              m_self;
        std::vector<pyobject_ref> m_stored_args;
        pyobject_ref              m_result;
    };

} // namespace mge::python
