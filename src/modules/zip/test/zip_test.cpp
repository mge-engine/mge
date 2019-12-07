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
    mge::archive::archive_entries e = a.entries();
    EXPECT_EQ(6u, e.size());
}

