#pragma once

#include <type_traits>

namespace mge::script {

    struct traits
    {
        unsigned int is_void : 1;
    };

    template <typename T> inline constexpr traits traits_of()
    {
        traits result{.is_void = std::is_same_v<T, void> ? 1 : 0};
        return result;
    }

} // namespace mge::script