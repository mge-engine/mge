#pragma once
#include "mge/shader/dllexport.hpp"
#include "mge/core/string_pool.hpp"
#include <iostream>

namespace mge {
namespace shader {

    class MGE_SHADER_EXPORT location
    {
    public:
        location(const char *file, int line, int column=0);
        location(const std::string& file, int line, int column=0);
        location(const location& l);
        location(location&& l);
        ~location();

        location& operator =(const location& l);
        location& operator =(location&& l);

        inline const char *file() const noexcept { return m_file; }
        int line() const noexcept { return m_line; }
        int column() const noexcept { return m_column; }
    private:
        const char *m_file;
        int         m_line;
        int         m_column;

        static string_pool s_files;
    };

    MGE_SHADER_EXPORT std::ostream& operator <<(std::ostream& os, const location& l);

}
}
