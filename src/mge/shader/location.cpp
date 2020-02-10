#include "mge/shader/location.hpp"
namespace mge {
namespace shader {

    location::location(const char *file, int line, int column)
        :m_file(s_files.intern(file))
        ,m_line(line)
        ,m_column(column)
    {}

    location::location(const std::string& file, int line, int column)
        :m_file(s_files.intern(file))
        ,m_line(line)
        ,m_column(column)
    {}

    location::location(const location &l)
        :m_file(l.m_file)
        ,m_line(l.m_line)
        ,m_column(l.m_column)
    {}

    location::location(location &&l)
        :m_file(std::move(l.m_file))
        ,m_line(std::move(l.m_line))
        ,m_column(std::move(l.m_column))
    {}

    location::~location()
    {}

    location&
    location::operator =(const location& l)
    {
        m_file   = l.m_file;
        m_line   = l.m_line;
        m_column = l.m_column;
        return *this;
    }

    string_pool
    location::s_files;
}
}
