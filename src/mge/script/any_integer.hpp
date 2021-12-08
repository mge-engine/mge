#pragma once
#include <cstdint>
#include <variant>
namespace mge::script {
    using any_integer = std::variant<int8_t,
                                     int16_t,
                                     int32_t,
                                     int64_t,
                                     uint8_t,
                                     uint16_t,
                                     uint32_t,
                                     uint64_t>;

}