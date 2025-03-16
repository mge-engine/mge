#pragma once
#include "mge/script/invocation_context.hpp"
#include "python.hpp"
#include "python_fwd.hpp"

namespace mge::python {

    class python_invocation_context : public mge::script::invocation_context
    {
    public:
        python_invocation_context(const python_type& type, PyObject* self);
        virtual ~python_invocation_context();

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
        void store_long_double_argument(size_t      index,
                                        long double value) override;
        void store_string_argument(size_t             index,
                                   const std::string& value) override;
        void
        store_object_argument(size_t                            index,
                              void*                             value,
                              const mge::script::type_data_ref& t) override;

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
        const python_type& m_type;
        PyObject*          m_self;
    };
} // namespace mge::python