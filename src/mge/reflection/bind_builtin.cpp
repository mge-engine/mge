#include "mge/reflection/type.hpp"

namespace mge {
    namespace reflection {
        void bind_builtin()
        {
            type<void>     t_void;
            type<uint8_t>  t_uint8_t;
            type<int8_t>   t_int8_t;
            type<uint16_t> t_uint16_t;
            type<int16_t>  t_int16_t;
            type<uint32_t> t_uint32_t;
            type<int32_t>  t_int32_t;
            type<uint64_t> t_uint64_t;
            type<int64_t>  t_int64_t;
            type<bool>     t_bool;
            type<float>    t_float;
            type<double>   t_double;
            type<long double>   t_long_double;
        }
    }
}
