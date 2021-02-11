// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/string_pool.hpp"
#include <string>
#include <string_view>
#include <vector>
namespace mge {

    /**
     * @brief A stack backtrace.
     *
     */
    class MGECORE_EXPORT stacktrace
    {
    public:
        class MGECORE_EXPORT frame
        {
        public:
            frame(const void *address, std::string_view name,
                  std::string_view file, uint32_t line);
            frame(const frame &) = default;

            ~frame() = default;

            frame &operator=(const frame &) = default;

            const void *address() const noexcept { return m_address; }

            std::string_view name() const noexcept { return m_name; }

            std::string_view source_file() const noexcept
            {
                return m_source_file;
            }

            uint32_t source_line() const noexcept { return m_source_line; }

        private:
            const void *     m_address;
            std::string_view m_name;
            std::string_view m_source_file;
            uint32_t         m_source_line;
        };

    private:
        using frame_vector = std::vector<frame>;

    public:
        using size_type = frame_vector::size_type;

        stacktrace();
        ~stacktrace() = default;

        size_type size() const;

    private:
        frame_vector m_frames;
        string_pool  m_strings;
    };

} // namespace mge