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

inline constexpr auto struct_ofs(const sview& sv) noexcept
{
    constexpr std::string_view struct_str = "struct ";
    constexpr std::string_view class_str = "class ";

    for (std::size_t i = 0; i < sv.sz_; ++i) {
        if (i + struct_str.size() <= sv.sz_) {
            if (std::string_view(sv.str_ + i, struct_str.size()) ==
                struct_str) {
                return i + struct_str.size();
            }
        }
        if (i + class_str.size() <= sv.sz_) {
            if (std::string_view(sv.str_ + i, class_str.size()) == class_str) {
                return i + class_str.size();
            }
        }
    }
    constexpr size_t result{0u};
    return result;
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
    constexpr auto ofs = struct_ofs(n);
    std::cout << std::string_view(n.str_ + ofs, l - ofs) << std::endl;
}