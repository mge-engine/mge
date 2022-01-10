#pragma once
#include <type_traits>
namespace mge {

    template <typename T> class is_callable
    {
    private:
        template <typename C> static char test(decltype(&C::operator()));
        template <typename C> static int  test(...);

    public:
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(char);
    };

} // namespace mge