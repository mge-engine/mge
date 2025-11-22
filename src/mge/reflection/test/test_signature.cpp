// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/signature.hpp"
#include "test/googletest.hpp"
#include "test/test_format.hpp"

namespace mge::reflection {

    TEST(signature, default_constructor)
    {
        signature sig;
        EXPECT_EQ(sig.return_type(), make_type_identifier<void>());
        EXPECT_TRUE(sig.parameter_types().empty());
    }

    TEST(signature, return_type_constructor)
    {
        auto      id = make_type_identifier<int>();
        signature sig(id);
        EXPECT_EQ(sig.return_type(), id);
        EXPECT_TRUE(sig.parameter_types().empty());
    }

    TEST(signature, full_constructor)
    {
        auto return_type = make_type_identifier<int>();
        std::vector<type_identifier> params = {make_type_identifier<float>(),
                                               make_type_identifier<double>()};
        signature sig(return_type, params);
        EXPECT_EQ(sig.return_type(), return_type);
        EXPECT_EQ(sig.parameter_types().size(), 2);
        EXPECT_EQ(sig.parameter_types()[0], make_type_identifier<float>());
        EXPECT_EQ(sig.parameter_types()[1], make_type_identifier<double>());
    }

    TEST(signature, hash_same_signature)
    {
        auto      return_type = make_type_identifier<int>();
        signature sig1(return_type);
        signature sig2(return_type);
        EXPECT_EQ(std::hash<signature>()(sig1), std::hash<signature>()(sig2));
    }

    TEST(signature, hash_different_return_type)
    {
        signature sig1(make_type_identifier<int>());
        signature sig2(make_type_identifier<float>());
        EXPECT_NE(std::hash<signature>()(sig1), std::hash<signature>()(sig2));
    }

    TEST(signature, hash_different_parameters)
    {
        auto return_type = make_type_identifier<int>();
        std::vector<type_identifier> params1 = {
            make_type_identifier<float>()};
        std::vector<type_identifier> params2 = {
            make_type_identifier<double>()};
        signature sig1(return_type, params1);
        signature sig2(return_type, params2);
        EXPECT_NE(std::hash<signature>()(sig1), std::hash<signature>()(sig2));
    }

    TEST(signature, hash_different_parameter_count)
    {
        auto return_type = make_type_identifier<int>();
        std::vector<type_identifier> params1 = {
            make_type_identifier<float>()};
        std::vector<type_identifier> params2 = {
            make_type_identifier<float>(),
            make_type_identifier<double>()};
        signature sig1(return_type, params1);
        signature sig2(return_type, params2);
        EXPECT_NE(std::hash<signature>()(sig1), std::hash<signature>()(sig2));
    }

    TEST(signature, format_void_no_params)
    {
        signature sig;
        mge::test_format(sig, "void()");
    }

    TEST(signature, format_int_no_params)
    {
        signature sig(make_type_identifier<int>());
        mge::test_format(sig, "int()");
    }

    TEST(signature, format_int_one_param)
    {
        std::vector<type_identifier> params = {
            make_type_identifier<float>()};
        signature sig(make_type_identifier<int>(), params);
        mge::test_format(sig, "int(float)");
    }

    TEST(signature, format_int_two_params)
    {
        std::vector<type_identifier> params = {make_type_identifier<float>(),
                                               make_type_identifier<double>()};
        signature sig(make_type_identifier<int>(), params);
        mge::test_format(sig, "int(float, double)");
    }

    TEST(signature, format_complex_types)
    {
        std::vector<type_identifier> params = {
            make_type_identifier<const int&>(),
            make_type_identifier<volatile float*>()};
        signature sig(make_type_identifier<const char*>(), params);
        // Note: On MSVC, pointer types include __ptr64 in their name
        auto formatted = fmt::format("{}", sig);
        EXPECT_TRUE(formatted.find("const int&") != std::string::npos);
        EXPECT_TRUE(formatted.find("float") != std::string::npos);
        EXPECT_TRUE(formatted.find("volatile") != std::string::npos);
    }

} // namespace mge::reflection
