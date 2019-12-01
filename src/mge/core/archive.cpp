#include "mge/core/archive.hpp"
#include "mge/core/archive_access_factory.hpp"
#include <map>

namespace mge {

    class factory_registry
    {
    public:
        factory_registry()
        {}

        ~factory_registry()
        {}
    private:
        std::map<std::string, archive_access_factory_ref> m_access_factories;
    };



    archive::entry::entry(const mge::path& p, bool dir)
        : m_path(p)
        , m_directory(dir)
    {}

    archive::entry::entry(const archive::entry& e)
        : m_path(e.m_path)
        , m_directory(e.m_directory)
    {}

    archive::entry::entry(archive::entry&& e)
        : m_path(std::move(e.m_path))
        , m_directory(e.m_directory)
    {}


    archive::entry& archive::entry::operator=(const archive::entry& e)
    {
        m_path = e.m_path;
        m_directory = e.m_directory;
        return *this;
    }

    archive::entry& archive::entry::operator=(archive::entry&& e)
    {
        m_path = std::move(e.m_path);
        m_directory = e.m_directory;
        return *this;
    }

    const mge::path& archive::entry::path() const
    {
        return m_path;
    }

    archive::archive(const file& f, archive::open_mode m)
        : m_file(f)
        , m_open_mode(m)
    {
        open();
    }

    archive::archive(const mge::path& p, archive::open_mode m)
        : m_file(p)
        , m_open_mode(m)
    {
        open();
    }

    archive::archive(const char *p, archive::open_mode m)
        : m_file(p)
        , m_open_mode(m)
    {
        open();
    }

    archive::~archive()
    {
    }

    void archive::open()
    {

    }

}
