// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mock_file_access.hpp"
#include "test/googlemock.hpp"
#include <memory>

using mge::file;
using mge::input_stream;
using mge::input_stream_ref;

TEST(file, exists_calls_file_access)
{
    std::shared_ptr<MOCK_file_access> mfa =
        std::make_shared<MOCK_file_access>("ignored");
    file f(mfa);
    EXPECT_CALL(*mfa, exists()).Times(1).WillRepeatedly(testing::Return(true));
    EXPECT_TRUE(f.exists());
}

TEST(file, is_file_calls_file_access)
{
    std::shared_ptr<MOCK_file_access> mfa =
        std::make_shared<MOCK_file_access>("ignored");
    file f(mfa);
    EXPECT_CALL(*mfa, is_file()).Times(1).WillRepeatedly(testing::Return(true));
    EXPECT_TRUE(f.is_file());
}

TEST(file, is_directory_calls_file_access)
{
    std::shared_ptr<MOCK_file_access> mfa =
        std::make_shared<MOCK_file_access>("ignored");
    file f(mfa);
    EXPECT_CALL(*mfa, is_directory())
        .Times(1)
        .WillRepeatedly(testing::Return(true));
    EXPECT_TRUE(f.is_directory());
}

TEST(file, is_system_file_calls_file_access)
{
    std::shared_ptr<MOCK_file_access> mfa =
        std::make_shared<MOCK_file_access>("ignored");
    file f(mfa);
    EXPECT_CALL(*mfa, is_system_file())
        .Times(1)
        .WillRepeatedly(testing::Return(true));
    EXPECT_TRUE(f.is_system_file());
}

// MinGW defines somewhere a "mkdir" function, which fools
// the EXPECT_CALL mechanism
#ifndef MGE_COMPILER_MINGW
TEST(file, mkdir_calls_file_access)
{
    std::shared_ptr<MOCK_file_access> mfa =
        std::make_shared<MOCK_file_access>("ignored");
    file f(mfa);
    EXPECT_CALL(*mfa, mkdir()).Times(1);
    f.mkdir();
}
#endif
#if 0
TEST(file, exists)
{
    file f("data");
    EXPECT_TRUE(f.exists());
    file f2("data/test.txt");
    EXPECT_TRUE(f2.exists());
    file f3("wurst");
    EXPECT_FALSE(f3.exists());
}

TEST(file, get_extension)
{
    file f2("data/test.txt");
    EXPECT_EQ(std::string("txt"), f2.get_extension());
}

TEST(file, is_directory)
{
    file f("data");
    EXPECT_TRUE(f.is_directory());
    file f2("data/test.txt");
    EXPECT_FALSE(f2.is_directory());
}

TEST(file, is_file)
{
    file f("data");
    EXPECT_FALSE(f.is_file());
    file f2("data/test.txt");
    EXPECT_TRUE(f2.is_file());
}

TEST(file, construct_with_path)
{
    file f("data", "test.txt");
    EXPECT_TRUE(f.is_file());
}

TEST(file, is_system_file)
{
    file f("data", "test.txt");
    EXPECT_TRUE(f.is_system_file());
}

TEST(file, mkdir_and_rmdir)
{
    file f("data", "blah");
    try {
        if (f.exists()) {
            f.rmdir();
        }
    } catch (...) {
        FAIL()<< "Cannot setup test: remove of directory etc/blah failed";
    }

    f.mkdir();
    EXPECT_TRUE(f.exists());
    EXPECT_TRUE(f.is_directory());
    f.rmdir();
    EXPECT_FALSE(f.exists());
}

TEST(file, mkdir_exists)
{
    file f("data");
    EXPECT_THROW(f.mkdir(), moge::filesystem_error);
}

TEST(file, list)
{
    file dir("data");
    std::vector<file> files;
    dir.list(files);
    EXPECT_TRUE(files.size() > 0);
    bool red_jpg_found = false;
    bool test_txt_found = false;
    for(auto f: files) {
        if(f.get_path() == "data/red.jpg") {
            red_jpg_found = true;
        }
        if(f.get_path() == "data/test.txt") {
            test_txt_found = true;
        }
    }
    if(!red_jpg_found) {
        FAIL() << "Expected \"data/red.jpg\" in list";
    }
    if(!test_txt_found) {
        FAIL() << "Expected \"data/test.txt\" in list";
    }

}

TEST(file, output)
{
    file f("data");
    std::stringstream ss;
    ss << f;
    EXPECT_EQ(std::string("data"), ss.str());
}

TEST(file, input_stream)
{
    file f("data", "test.txt");
    input_stream_ref is = f.open_for_input();
    is->seek(5, input_stream::POS_BEG);
    EXPECT_EQ(5, is->position());
}

TEST(file, binary_input)
{
    FILE *f = fopen("data/red.jpg", "rb");
    char binary[512];
    size_t binary_size = fread(binary, 1, 512, f);
    EXPECT_EQ(382u, binary_size);
    fclose(f);

    file fi("data/red.jpg");
    input_stream_ref is = fi.open_for_input();
    char binary2[512];
    input_stream::streamsize_type binary2size = is->read(binary2, 512);
    EXPECT_EQ(382, binary2size);
    EXPECT_TRUE(memcmp(binary, binary2, 382)==0);
}
#endif
