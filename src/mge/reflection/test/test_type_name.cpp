#include "test/googletest.hpp"
#include <string_view>
#include <type_traits>

struct sview
{
    const char* str_;
    std::size_t sz_;
};

template <typename V> inline constexpr auto v() noexcept
{
#ifdef _MSC_VER
    return sview{__FUNCSIG__ + 15, sizeof(__FUNCSIG__) - 32};
#else
#    error Not implemented
#endif
}

TEST(typename, test_int)
{
    //"auto __cdecl v<int>(void) noexcept";
    auto n = v<int>();
    std::cout << std::string_view(n.str_, n.sz_) << std::endl;
    auto n2 = v<const int>();
    std::cout << std::string_view(n2.str_, n2.sz_) << std::endl;
    auto n3 = v<int&>();
    std::cout << std::string_view(n3.str_, n3.sz_) << std::endl;
    auto n4 = v<const int&>();
    std::cout << std::string_view(n4.str_, n4.sz_) << std::endl;
    auto n5 = v<int&&>();
    std::cout << std::string_view(n5.str_, n5.sz_) << std::endl;
    auto n6 = v<int*>();
    std::cout << std::string_view(n6.str_, n6.sz_) << std::endl;
    auto n7 = v<std::string_view>();
    std::cout << std::string_view(n7.str_, n7.sz_) << std::endl;
}