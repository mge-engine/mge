#include "mge/config.hpp"
#include "mge/core/enum.hpp"
#include "test/googletest.hpp"
#include <cstdint>
#include <source_location>
#include <type_traits>

enum class test_enum : uint8_t
{
    FOO = 0,
    FOO_BAR,
    FOO_FOO_BAR
};

TEST(enum_test, simple_enum)
{
    test_enum e = test_enum::FOO;
    auto      s = std::format("{}", e);
    EXPECT_STREQ("FOO", s.c_str());

    test_enum bad = static_cast<test_enum>(0xff);
    auto      sbad = std::format("{}", bad);
    EXPECT_STREQ("(test_enum)0xff", sbad.c_str());
    auto i = mge::enum_index(bad);
    std::cout << i.has_value() << std::endl;
}

enum class custom
{
    A = 1111,
    B = 2222,
};

template <> struct mge::enum_range<custom>
{
    static constexpr size_t min = 0;
    static constexpr size_t max = 4000;
};

TEST(enum_test, customize)
{
    custom c = custom::A;
    auto   s = std::format("{}", c);
    EXPECT_STREQ("A", s.c_str());
}