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

inline constexpr auto last_sep(const sview& sv) noexcept
{
    for (std::size_t i = sv.sz_; i > 0; --i) {
        if (sv.str_[i - 1] == ':') {
            if (i >= 2 && sv.str_[i - 2] == ':') {
                return i - 2;
            }
        }
    }
    return std::string::npos;
}
namespace x {
    namespace y {
        struct z
        {};
    } // namespace y
} // namespace x

TEST(typename, test_int)
{
    //"auto __cdecl v<int>(void) noexcept";
    constexpr auto n = v<x::y::z>();
    constexpr auto l = last_sep(n);
    std::cout << std::string_view(n.str_, l) << std::endl;
}