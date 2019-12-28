#include "test/googletest.hpp"
#include "mge/core/file.hpp"
#include "mge/core/archive.hpp"

using namespace testing;

TEST(ziptest, zipfile_found)
{
    mge::file zipfile("assets/archive.zip");
    EXPECT_TRUE(zipfile.exists()) << "Expect file assets/archive.zip located in relative path";
}

TEST(ziptest, open_archive)
{
    mge::archive a("assets/archive.zip", mge::open_mode::READ);
}

TEST(ziptest, archive_entries)
{
    mge::archive a("assets/archive.zip", mge::open_mode::READ);
    mge::archive::archive_entries ae = a.entries();
    EXPECT_EQ(6u, ae.size());
    for(const auto &e : ae) {
        if (e.path() == mge::path("dir1") 
            || e.path() == mge::path("dir1/dira")
            || e.path() == mge::path("dir1/dirb")) {
            EXPECT_TRUE(e.is_directory());
        } else {
            EXPECT_FALSE(e.is_directory());
        }
    }
}

TEST(ziptest, find_entry)
{
    mge::archive a("assets/archive.zip", mge::open_mode::READ);
    auto e = a.find_entry("dir1/dira/b.txt");
    EXPECT_EQ(9u, e.size());
}

TEST(ziptest, read_entry)
{
    mge::archive a("assets/archive.zip", mge::open_mode::READ);
    auto e = a.find_entry("dir1/dira/b.txt");
    auto is = e.open();
    std::string str;
    is->istream() >> str;
    EXPECT_EQ(std::string(""), str);
}

