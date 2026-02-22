// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform.hpp"
#include "test/googletest.hpp"

using namespace mge;

TEST(uniform, basic_construction)
{
    uniform<int32_t> u("test_uniform");
    EXPECT_EQ(u.type(), uniform_data_type::INT32);
    EXPECT_EQ(u.version(), 0u);
}

TEST(uniform, float_assignment)
{
    uniform<float> u("float_uniform");
    uint64_t       initial_version = u.version();

    u = 3.14f;

    EXPECT_EQ(u.m_value, 3.14f);
    EXPECT_GT(u.version(), initial_version);
}

TEST(uniform, int_assignment)
{
    uniform<int32_t> u("int_uniform");
    uint64_t         initial_version = u.version();

    u = 42;

    EXPECT_EQ(u.m_value, 42);
    EXPECT_GT(u.version(), initial_version);
}

TEST(uniform, version_increments)
{
    uniform<int32_t> u("version_test");
    uint64_t         v0 = u.version();

    u = 1;
    uint64_t v1 = u.version();
    EXPECT_GT(v1, v0);

    u = 2;
    uint64_t v2 = u.version();
    EXPECT_GT(v2, v1);

    u = 3;
    uint64_t v3 = u.version();
    EXPECT_GT(v3, v2);
}

TEST(uniform, double_type)
{
    uniform<double> u("double_uniform");
    EXPECT_EQ(u.type(), uniform_data_type::DOUBLE);

    u = 2.718281828;
    EXPECT_EQ(u.m_value, 2.718281828);
}

TEST(uniform, multiple_uniforms)
{
    uniform<int32_t> u1("uniform_1");
    uniform<float>   u2("uniform_2");
    uniform<double>  u3("uniform_3");

    EXPECT_EQ(u1.type(), uniform_data_type::INT32);
    EXPECT_EQ(u2.type(), uniform_data_type::FLOAT);
    EXPECT_EQ(u3.type(), uniform_data_type::DOUBLE);

    u1 = 10;
    u2 = 20.0f;
    u3 = 30.0;

    EXPECT_EQ(u1.m_value, 10);
    EXPECT_EQ(u2.m_value, 20.0f);
    EXPECT_EQ(u3.m_value, 30.0);
}

TEST(uniform, vector_type)
{
    uniform<fvec3> u("vec3_uniform");
    EXPECT_EQ(u.type(), uniform_data_type::FLOAT_VEC3);
}

TEST(uniform, scoped_destruction)
{
    {
        uniform<int32_t> u1("scoped_uniform_1");
        u1 = 42;
        EXPECT_EQ(u1.m_value, 42);
    }

    {
        uniform<float> u2("scoped_uniform_2");
        u2 = 3.14f;
        EXPECT_EQ(u2.m_value, 3.14f);
    }
}
