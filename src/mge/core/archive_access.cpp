#include "mge/core/archive_access.hpp"
#include <iostream>
namespace mge {

    archive_access::archive_access(const file& f, open_mode m)
        : m_file(f)
        , m_open_mode(m)
    {}

    void archive_access::gist(std::ostream& os)
    {
        
    }


}