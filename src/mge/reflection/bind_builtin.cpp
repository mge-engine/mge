#include "mge/reflection/type.hpp"

namespace mge {
    namespace reflection {
        void bind_builtin()
        {
            type::get<void>();

            type::get<uint8_t>();
            type::get<int8_t>();

            type::get<int16_t>();
            type::get<uint16_t>();

            type::get<int32_t>();
            type::get<uint32_t>();


            type::get<int64_t>();
            type::get<uint64_t>();

            type::get<bool>();

            type::get<float>();
            type::get<double>();
            type::get<long double>();

            type::get<const char *>();
            type::get<char *>();
        }
    }
}
