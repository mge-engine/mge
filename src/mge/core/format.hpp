#pragma once
#include <format>
#include <iostream>
#include <string_view>
#include <type_traits>

namespace mge {

    template <typename T>
    concept has_format_method = requires(T obj, std::format_context& ctx) {
        {
            obj.format(ctx)
        } -> std::convertible_to<void>;
    };

} // namespace mge

template <typename T, typename C>
    requires mge::has_format_method<T>
struct std::formatter<T, C> : public std::formatter<std::string_view, C>
{
    template <typename FormatContext>
    auto format(const T& t, FormatContext& ctx) const
    {
        t.format(ctx);
        return ctx.out();
    }
};

namespace mge {

    template <typename T>
        requires mge::has_format_method<T>
    inline std::ostream& operator<<(std::ostream& os, const T& t)
    {
        std::ostream_iterator<char> out(os);
        std::format_to(out, "{}", t);
        return os;
    }
} // namespace mge
