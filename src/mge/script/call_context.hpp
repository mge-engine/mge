#pragma once
#include "mge/core/exception.hpp"
#include "mge/script/dllexport.hpp"
#include "mge/script/script_fwd.hpp"


#include <functional>
#include <string>
namespace mge::script {

    /**
     * Context of a call.
     *
     * When a script calls a function or method, is used to request the
     * parameter values.
     */
    class MGESCRIPT_EXPORT call_context
    {
    protected:
        call_context() = default;

    public:
        virtual ~call_context() = default;

        virtual void store_bool_result(bool result) = 0;
        virtual void store_int8_t_result(int8_t result) = 0;
        virtual void store_uint8_t_result(uint8_t result) = 0;
        virtual void store_int16_t_result(int16_t result) = 0;
        virtual void store_uint16_t_result(uint16_t result) = 0;
        virtual void store_int32_t_result(int32_t result) = 0;
        virtual void store_uint32_t_result(uint32_t result) = 0;
        virtual void store_int64_t_result(int64_t result) = 0;
        virtual void store_uint64_t_result(uint64_t result) = 0;
        virtual void store_float_result(float result) = 0;
        virtual void store_double_result(double result) = 0;
        virtual void store_long_double_result(long double result) = 0;
        virtual void store_string_result(const std::string& result) = 0;

        virtual void store_object_result(void*                result,
                                         const type_data_ref& t) = 0;

        virtual void exception_thrown(const std::exception& e) = 0;
        virtual void exception_thrown(const mge::exception& e) = 0;
        virtual void exception_thrown() = 0;
    };

    using invoke_function = std::function<void(call_context&)>;

} // namespace mge::script