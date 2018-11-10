// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googlemock.hpp"
#include "mge/core/file_access.hpp"
#include "mge/core/file.hpp"

class MOCK_file_access : public mge::file_access
{
public:
    MOCK_file_access(const std::string& path)
        :mge::file_access(path)
    {}

    MOCK_file_access() = default;
    virtual ~MOCK_file_access() = default;

    MOCK_CONST_METHOD0(exists, bool());
    MOCK_CONST_METHOD0(is_file, bool());
    MOCK_CONST_METHOD0(is_directory, bool());
    MOCK_CONST_METHOD0(is_system_file, bool());
    MOCK_METHOD0(mkdir, void());
    MOCK_METHOD0(rmdir, void());
    MOCK_METHOD1(list, void(std::vector<mge::file>&));

    MOCK_CONST_METHOD0(open_for_input, mge::input_stream_ref());

    void set_path(const std::string& path)
    {
        mge::file_access::set_path(path);
    }
};
