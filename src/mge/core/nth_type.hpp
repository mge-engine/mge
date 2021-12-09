#pragma once
#include <tuple>
namespace mge {
    /**
     * @brief Return the n-th type of a type pack.
     *
     * @tparam N index in type pack
     * @tparam Ts type pack
     */
    template <std::size_t N, typename... Ts>
    using nth_type = typename std::tuple_element<N, std::tuple<Ts...>>::type;
}