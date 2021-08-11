#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/small_vector.hpp"
#include <array>
#include <cstdint>
#include <ranges>
#include <vector>

namespace mge {

    using buffer = std::vector<uint8_t>;

    template <size_t N> using array_buffer = std::array<uint8_t, N>;

    template <size_t N> using small_buffer = mge::small_vector<uint8_t, N>;

} // namespace mge