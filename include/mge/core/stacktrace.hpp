// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <string>
#include <string_view>

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
            frame(void *address, std::string_view name, std::string_view file,
                  uint32_t line);
            ~frame();

        private:
            void *           m_address;
            std::string_view m_name;
            std::string_view m_file;
            uint32_t         m_line;
        };
    };

} // namespace mge