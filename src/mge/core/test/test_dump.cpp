// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/crash.hpp"
#include "mge/core/dump.hpp"
#include "mge/core/dump_info_provider.hpp"
#include "mge/core/markdown_document.hpp"
#include "test/googletest.hpp"

#include <filesystem>
#include <memory_resource>

namespace {

    class test_provider : public mge::dump_info_provider
    {
    public:
        test_provider() = default;
        test_provider(std::pmr::memory_resource* resource)
            : dump_info_provider(resource)
        {}

        std::pmr::string section_name() const override
        {
            return std::pmr::string("Test Section", m_resource);
        }

        void dump_info(mge::markdown_document& doc) const override
        {
            doc.paragraph("Test info.");
        }
    };

    class empty_provider : public mge::dump_info_provider
    {
    public:
        empty_provider() = default;
        empty_provider(std::pmr::memory_resource* resource)
            : dump_info_provider(resource)
        {}

        std::pmr::string section_name() const override
        {
            return std::pmr::string("Empty Section", m_resource);
        }

        bool has_info() const override
        {
            return false;
        }

        void dump_info(mge::markdown_document& doc) const override
        {
            doc.paragraph("Should not appear.");
        }
    };

} // namespace

TEST(dump_info_provider, has_info_default_true)
{
    test_provider p;
    EXPECT_TRUE(p.has_info());
}

TEST(dump_info_provider, has_info_override_false)
{
    empty_provider p;
    EXPECT_FALSE(p.has_info());
}

TEST(dump_info_provider, section_name)
{
    test_provider p;
    EXPECT_EQ("Test Section", p.section_name());
}

TEST(dump_info_provider, section_name_with_resource)
{
    std::pmr::monotonic_buffer_resource mbr;
    test_provider                       p(&mbr);
    auto                                name = p.section_name();
    EXPECT_EQ("Test Section", name);
    EXPECT_EQ(name.get_allocator().resource(), &mbr);
}

TEST(dump_info_provider, dump_info_writes_to_doc)
{
    test_provider          p;
    mge::markdown_document doc;
    p.dump_info(doc);
    auto result = doc.str();
    EXPECT_NE(result.find("Test info."), std::pmr::string::npos);
}

TEST(dump, construct)
{
    mge::dump d;
}

TEST(dump, construct_with_resource)
{
    std::pmr::monotonic_buffer_resource mbr;
    mge::dump                           d("test", &mbr);
}

TEST(dump, write)
{
    namespace fs = std::filesystem;

    // Remove any pre-existing crash dump files
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().filename().string().find("-crash-") !=
                std::string::npos &&
            entry.path().extension() == ".md") {
            fs::remove(entry.path());
        }
    }

    mge::dump d("crash");
    d.write();

    bool found = false;
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().filename().string().find("-crash-") !=
                std::string::npos &&
            entry.path().extension() == ".md") {
            EXPECT_GT(fs::file_size(entry.path()), 0u);
            found = true;
            // fs::remove(entry.path());
        }
    }
    EXPECT_TRUE(found);
}

TEST(crash, writes_dump_and_aborts)
{
    namespace fs = std::filesystem;

    // Remove any pre-existing crash dump files
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().filename().string().find("-crash-") !=
                std::string::npos &&
            entry.path().extension() == ".md") {
            fs::remove(entry.path());
        }
    }

    EXPECT_DEATH(mge::crash(), "Crash!!!");

    bool found = false;
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().filename().string().find("-crash-") !=
                std::string::npos &&
            entry.path().extension() == ".md") {
            EXPECT_GT(fs::file_size(entry.path()), 0u);
            found = true;
            fs::remove(entry.path());
        }
    }
    EXPECT_TRUE(found);
}
