// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "asset_test.hpp"
#include "mge/asset/asset.hpp"
#include "mge/asset/asset_source.hpp"
#include "mge/asset/db_asset_access.hpp"
#include <filesystem>

class test_db_asset : public mge::asset_test
{
protected:
    void SetUp() override
    {
        m_db_path =
            std::filesystem::temp_directory_path() / "test_mge_db_asset.db";
        std::filesystem::remove(m_db_path);
        mge::properties p;
        p.put("path", m_db_path.string());
        mge::asset::mount("/db",
                          "db",
                          mge::asset_source::access_mode::READ_WRITE,
                          p);
    }

    void TearDown() override
    {
        mge::asset::unmount("/db");
        std::filesystem::remove(m_db_path);
    }

    std::filesystem::path m_db_path;
};

TEST_F(test_db_asset, mount_unmount) {}

TEST_F(test_db_asset, write_and_read)
{
    {
        mge::asset       a("/db/hello.txt");
        auto             out = a.output_stream();
        std::string_view data = "hello world";
        out->write(data.data(), data.size());
        out->flush();
    }

    mge::asset b("/db/hello.txt");
    EXPECT_EQ(11u, b.size());
    auto        in = b.data();
    std::string result(11, '\0');
    in->read(result.data(), result.size());
    EXPECT_EQ("hello world", result);
}

TEST_F(test_db_asset, unwritten_asset_has_zero_size)
{
    mge::asset a("/db/nonexistent.dat");
    EXPECT_EQ(0u, a.size());
}

TEST_F(test_db_asset, asset_exists_in_source)
{
    mge::asset a("/db/check.dat");
    {
        auto out = a.output_stream();
        out->write("x", 1);
        out->flush();
    }
    mge::db_asset_access acc("/db/check.dat", m_db_path);
    EXPECT_EQ(1u, acc.size());
}

TEST_F(test_db_asset, store_and_read_properties)
{
    {
        mge::asset a("/db/props.dat");
        auto       out = a.output_stream();
        out->write("data", 4);
        out->flush();
    }

    mge::db_asset_access acc("/db/props.dat", m_db_path);
    EXPECT_FALSE(acc.has_properties());

    mge::properties p;
    p.put("color", "red");
    p.put("size", "42");
    acc.store_properties(p);

    EXPECT_TRUE(acc.has_properties());
    auto loaded = acc.properties();
    EXPECT_EQ("red", loaded->get<std::string>("color"));
    EXPECT_EQ("42", loaded->get<std::string>("size"));
}
