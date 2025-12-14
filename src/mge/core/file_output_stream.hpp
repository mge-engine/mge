// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/output_stream.hpp"
#include "mge/core/path.hpp"

namespace mge {

    class MGECORE_EXPORT file_output_stream : public output_stream
    {
    public:
        file_output_stream(const path& path);
        virtual ~file_output_stream();

    private:
        void on_write(const void* buffer, streamsize_type size) override;
        void on_flush() override;

        path  m_path;
        FILE* m_file{nullptr};
    };

} // namespace mge
