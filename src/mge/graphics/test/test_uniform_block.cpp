// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/std140.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "test/googletest.hpp"

// --- std140 alignment tests ---

TEST(std140, float_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::FLOAT), 4u);
}

TEST(std140, int32_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::INT32), 4u);
}

TEST(std140, float_vec2_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::FLOAT_VEC2),
              8u);
}

TEST(std140, float_vec3_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::FLOAT_VEC3),
              16u);
}

TEST(std140, float_vec4_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::FLOAT_VEC4),
              16u);
}

TEST(std140, float_mat4_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::FLOAT_MAT4),
              16u);
}

TEST(std140, double_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::DOUBLE), 8u);
}

TEST(std140, double_vec3_alignment)
{
    EXPECT_EQ(mge::std140_base_alignment(mge::uniform_data_type::DOUBLE_VEC3),
              32u);
}

// --- std140 size tests ---

TEST(std140, float_size)
{
    EXPECT_EQ(mge::std140_type_size(mge::uniform_data_type::FLOAT), 4u);
}

TEST(std140, float_vec3_size)
{
    EXPECT_EQ(mge::std140_type_size(mge::uniform_data_type::FLOAT_VEC3), 12u);
}

TEST(std140, float_vec4_size)
{
    EXPECT_EQ(mge::std140_type_size(mge::uniform_data_type::FLOAT_VEC4), 16u);
}

TEST(std140, float_mat4_size)
{
    // 4 columns x 16 bytes per column
    EXPECT_EQ(mge::std140_type_size(mge::uniform_data_type::FLOAT_MAT4), 64u);
}

TEST(std140, float_mat3_size)
{
    // 3 columns x 16 bytes per column (vec3 padded to vec4)
    EXPECT_EQ(mge::std140_type_size(mge::uniform_data_type::FLOAT_MAT3), 48u);
}

TEST(std140, float_mat2_size)
{
    // 2 columns x 16 bytes per column (vec2 padded to vec4 stride)
    EXPECT_EQ(mge::std140_type_size(mge::uniform_data_type::FLOAT_MAT2), 32u);
}

// --- std140 array stride tests ---

TEST(std140, float_array_stride)
{
    // float array: each element padded to 16 bytes
    EXPECT_EQ(mge::std140_array_stride(mge::uniform_data_type::FLOAT), 16u);
}

TEST(std140, float_vec4_array_stride)
{
    // vec4 array: 16 bytes per element (already 16-aligned)
    EXPECT_EQ(mge::std140_array_stride(mge::uniform_data_type::FLOAT_VEC4),
              16u);
}

TEST(std140, float_mat4_array_stride)
{
    // mat4 array: 64 bytes per element (already 16-aligned)
    EXPECT_EQ(mge::std140_array_stride(mge::uniform_data_type::FLOAT_MAT4),
              64u);
}

// --- std140 align helper ---

TEST(std140, align_offsets)
{
    EXPECT_EQ(mge::std140_align(0, 16), 0u);
    EXPECT_EQ(mge::std140_align(1, 16), 16u);
    EXPECT_EQ(mge::std140_align(16, 16), 16u);
    EXPECT_EQ(mge::std140_align(17, 16), 32u);
    EXPECT_EQ(mge::std140_align(0, 4), 0u);
    EXPECT_EQ(mge::std140_align(3, 4), 4u);
}

// --- uniform_block tests ---

TEST(uniform_block, create_from_buffer_info)
{
    mge::program::uniform_block_metadata ub_info;
    ub_info.name = "TestBlock";
    ub_info.uniforms.push_back(
        {"model", mge::uniform_data_type::FLOAT_MAT4, 1, 0});
    ub_info.uniforms.push_back(
        {"color", mge::uniform_data_type::FLOAT_VEC4, 1, 0});

    mge::uniform_block block(ub_info);

    EXPECT_EQ(block.name(), "TestBlock");
    // mat4 = 64 bytes at offset 0, vec4 = 16 bytes at offset 64
    // total = 80, rounded to 16 = 80
    EXPECT_EQ(block.data_size(), 80u);
    EXPECT_EQ(block.version(), 0u);
    EXPECT_EQ(block.members().size(), 2u);
    EXPECT_EQ(block.members()[0].name, "model");
    EXPECT_EQ(block.members()[0].offset, 0u);
    EXPECT_EQ(block.members()[0].size, 64u);
    EXPECT_EQ(block.members()[1].name, "color");
    EXPECT_EQ(block.members()[1].offset, 64u);
    EXPECT_EQ(block.members()[1].size, 16u);
}

TEST(uniform_block, set_data_increments_version)
{
    mge::program::uniform_block_metadata ub_info;
    ub_info.name = "VersionBlock";
    ub_info.uniforms.push_back({"value", mge::uniform_data_type::FLOAT, 1, 0});

    mge::uniform_block block(ub_info);
    EXPECT_EQ(block.version(), 0u);

    float v = 42.0f;
    block.set_data("value", &v, sizeof(v));
    EXPECT_EQ(block.version(), 1u);

    v = 99.0f;
    block.set_data("value", &v, sizeof(v));
    EXPECT_EQ(block.version(), 2u);
}

TEST(uniform_block, set_typed_value)
{
    mge::program::uniform_block_metadata ub_info;
    ub_info.name = "TypedBlock";
    ub_info.uniforms.push_back(
        {"intensity", mge::uniform_data_type::FLOAT, 1, 0});

    mge::uniform_block block(ub_info);
    block.set<float>("intensity", 0.75f);

    const float* data = reinterpret_cast<const float*>(
        static_cast<const char*>(block.data()) + block.members()[0].offset);
    EXPECT_FLOAT_EQ(*data, 0.75f);
}

TEST(uniform_block, set_unknown_member_throws)
{
    mge::program::uniform_block_metadata ub_info;
    ub_info.name = "Block";
    ub_info.uniforms.push_back({"x", mge::uniform_data_type::FLOAT, 1, 0});

    mge::uniform_block block(ub_info);
    float              v = 1.0f;

    EXPECT_THROW(block.set_data("nonexistent", &v, sizeof(v)),
                 mge::no_such_element);
}

TEST(uniform_block, layout_mixed_types)
{
    // Block: float, vec3, mat4
    // float at offset 0, size 4
    // vec3 at offset 16 (aligned to 16), size 12
    // mat4 at offset 32 (aligned to 16), size 64
    // total = 96, rounded to 16 = 96
    mge::program::uniform_block_metadata ub_info;
    ub_info.name = "MixedBlock";
    ub_info.uniforms.push_back(
        {"brightness", mge::uniform_data_type::FLOAT, 1, 0});
    ub_info.uniforms.push_back(
        {"position", mge::uniform_data_type::FLOAT_VEC3, 1, 0});
    ub_info.uniforms.push_back(
        {"transform", mge::uniform_data_type::FLOAT_MAT4, 1, 0});

    mge::uniform_block block(ub_info);

    EXPECT_EQ(block.members()[0].offset, 0u);
    EXPECT_EQ(block.members()[1].offset, 16u);
    EXPECT_EQ(block.members()[2].offset, 32u);
    EXPECT_EQ(block.data_size(), 96u);
}

TEST(uniform_block, move_construct)
{
    mge::program::uniform_block_metadata ub_info;
    ub_info.name = "MoveBlock";
    ub_info.uniforms.push_back({"val", mge::uniform_data_type::FLOAT, 1, 0});

    mge::uniform_block block1(ub_info);
    block1.set<float>("val", 3.14f);

    mge::uniform_block block2(std::move(block1));
    EXPECT_EQ(block2.name(), "MoveBlock");
    EXPECT_EQ(block2.version(), 1u);

    const float* data = reinterpret_cast<const float*>(block2.data());
    EXPECT_FLOAT_EQ(*data, 3.14f);
}
