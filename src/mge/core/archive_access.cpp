#include "mge/core/archive_access.hpp"
namespace mge {

    archive_access::archive_access(const file& f, open_mode m)
        : m_file(f)
        , m_open_mode(m)
    {}


}