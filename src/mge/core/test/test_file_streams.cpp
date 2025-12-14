// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/file_input_stream.hpp"
#include "mge/core/file_output_stream.hpp"
#include "test/googletest.hpp"
#include <filesystem>
#include <fstream>

class file_streams_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_test_file =
            std::filesystem::temp_directory_path() / "mge_test_file.bin";
        // Clean up if file exists from previous test
        if (std::filesystem::exists(m_test_file)) {
            std::filesystem::remove(m_test_file);
        }
    }

    void TearDown() override
    {
        // Clean up test file
        if (std::filesystem::exists(m_test_file)) {
            std::filesystem::remove(m_test_file);
        }
    }

    std::filesystem::path m_test_file;
};

TEST_F(file_streams_test, output_stream_write_byte)
{
    {
        mge::file_output_stream output(m_test_file);
        output.write(static_cast<uint8_t>('A'));
        output.flush();
    }

    // Verify file was written
    std::ifstream file(m_test_file, std::ios::binary);
    ASSERT_TRUE(file.is_open());
    char byte;
    file.read(&byte, 1);
    EXPECT_EQ('A', byte);
}

TEST_F(file_streams_test, output_stream_write_buffer)
{
    const char* data = "Hello, World!";
    size_t      len = strlen(data);

    {
        mge::file_output_stream output(m_test_file);
        output.write(data, len);
        output.flush();
    }

    // Verify file was written
    std::ifstream file(m_test_file, std::ios::binary);
    ASSERT_TRUE(file.is_open());
    std::string content;
    content.resize(len);
    file.read(&content[0], len);
    EXPECT_EQ("Hello, World!", content);
}

TEST_F(file_streams_test, output_stream_ostream)
{
    {
        mge::file_output_stream output(m_test_file);
        std::ostream&           os = output.ostream();
        os << "Test data: " << 42 << std::endl;
        os.flush();
    }

    // Verify file was written
    std::ifstream file(m_test_file);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::getline(file, line);
    EXPECT_EQ("Test data: 42", line);
}

TEST_F(file_streams_test, input_stream_read_byte)
{
    // Create test file
    {
        std::ofstream file(m_test_file, std::ios::binary);
        file.write("A", 1);
    }

    mge::file_input_stream input(m_test_file);
    EXPECT_EQ('A', input.read());
}

TEST_F(file_streams_test, input_stream_read_buffer)
{
    const char* data = "Hello, World!";
    size_t      len = strlen(data);

    // Create test file
    {
        std::ofstream file(m_test_file, std::ios::binary);
        file.write(data, len);
    }

    mge::file_input_stream input(m_test_file);
    char                   buffer[64] = {0};
    auto                   read_count = input.read(buffer, len);
    EXPECT_EQ(len, read_count);
    EXPECT_STREQ("Hello, World!", buffer);
}

TEST_F(file_streams_test, input_stream_position_and_seek)
{
    const char* data = "0123456789";
    size_t      len = strlen(data);

    // Create test file
    {
        std::ofstream file(m_test_file, std::ios::binary);
        file.write(data, len);
    }

    mge::file_input_stream input(m_test_file);

    // Read first byte
    EXPECT_EQ('0', input.read());
    EXPECT_EQ(1, input.position());

    // Seek to middle
    input.seek(5, mge::input_stream::POS_BEG);
    EXPECT_EQ(5, input.position());
    EXPECT_EQ('5', input.read());

    // Seek from current position
    input.seek(2, mge::input_stream::POS_CUR);
    EXPECT_EQ(8, input.position());
    EXPECT_EQ('8', input.read());

    // Seek from end
    input.seek(-1, mge::input_stream::POS_END);
    EXPECT_EQ(9, input.position());
    EXPECT_EQ('9', input.read());
}

TEST_F(file_streams_test, roundtrip_write_and_read)
{
    const char* test_data = "The quick brown fox jumps over the lazy dog";
    size_t      len = strlen(test_data);

    // Write data
    {
        mge::file_output_stream output(m_test_file);
        output.write(test_data, len);
        output.flush();
    }

    // Read data back
    {
        mge::file_input_stream input(m_test_file);
        char                   buffer[256] = {0};
        auto                   read_count = input.read(buffer, len);
        EXPECT_EQ(len, read_count);
        EXPECT_STREQ(test_data, buffer);
    }
}

TEST_F(file_streams_test, output_stream_write_listener)
{
    mge::file_output_stream             output(m_test_file);
    mge::output_stream::streamsize_type total_written = 0;

    auto listener_key = output.add_write_listener(
        [&](mge::output_stream::streamsize_type bytes) {
            total_written += bytes;
        });

    output.write("Hello", 5);
    output.write(", ", 2);
    output.write("World!", 6);

    output.remove_write_listener(listener_key);

    EXPECT_EQ(13, total_written);
}

TEST_F(file_streams_test, input_stream_read_listener)
{
    // Create test file
    {
        std::ofstream file(m_test_file, std::ios::binary);
        file.write("Test data", 9);
    }

    mge::file_input_stream             input(m_test_file);
    mge::input_stream::streamsize_type total_read = 0;

    auto listener_key = input.add_read_listener(
        [&](mge::input_stream::streamsize_type bytes) { total_read += bytes; });

    char buffer[10] = {0};
    input.read(buffer, 9);

    input.remove_read_listener(listener_key);

    EXPECT_EQ(9, total_read);
    EXPECT_STREQ("Test data", buffer);
}

TEST_F(file_streams_test, output_stream_throws_on_invalid_path)
{
    std::filesystem::path invalid_path =
        std::filesystem::path("/invalid/path/that/does/not/exist/file.bin");

    EXPECT_THROW(mge::file_output_stream output(invalid_path), mge::exception);
}

TEST_F(file_streams_test, input_stream_throws_on_nonexistent_file)
{
    std::filesystem::path nonexistent =
        std::filesystem::temp_directory_path() / "nonexistent_file.bin";

    EXPECT_THROW(mge::file_input_stream input(nonexistent), mge::exception);
}

TEST_F(file_streams_test, binary_data_roundtrip)
{
    // Write binary data
    {
        mge::file_output_stream output(m_test_file);
        for (int i = 0; i < 256; ++i) {
            output.write(static_cast<uint8_t>(i));
        }
        output.flush();
    }

    // Read binary data back
    {
        mge::file_input_stream input(m_test_file);
        for (int i = 0; i < 256; ++i) {
            auto byte = input.read();
            EXPECT_EQ(i, static_cast<int>(byte));
        }
    }
}
