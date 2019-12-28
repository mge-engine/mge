#include "mge/core/archive.hpp"
#include "mge/core/archive_access_factory.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/log.hpp"

#include <map>

MGE_DEFINE_LOG(ARCHIVE);

namespace mge {

    class factory_registry
    {
    public:
        factory_registry()
        {}

        ~factory_registry()
        {}

        archive_access_factory_ref get_factory(const file& f)
        {
            auto fac = find_factory(f.path());
            if (!fac) {
                update_factories();
                fac = find_factory(f.path());
            }
            return fac;
        }
    private:
        void update_factories()
        {
            archive_access_factory::implementations([&](const std::string& n) { 
                std::lock_guard<std::mutex> guard(m_lock);
                if (m_access_factories.find(n) == m_access_factories.end()) {
                    MGE_DEBUG_LOG(ARCHIVE) << "Create archive access for " << n;
                    m_access_factories[n] = archive_access_factory::create(n);
                }
            });
        }

        archive_access_factory_ref find_factory(const path& p)
        {
            MGE_DEBUG_LOG(ARCHIVE) << "Find archive handler for " << p;
            std::lock_guard<std::mutex> guard(m_lock);
            for (const auto& e : m_access_factories) {
                MGE_DEBUG_LOG(ARCHIVE) << "Check " << e.first;
                if (e.second->handles_path(p)) {
                    return e.second;
                }
            }
            return archive_access_factory_ref();
        }

        std::mutex                                        m_lock;
        std::map<std::string, archive_access_factory_ref> m_access_factories;
    };

    static singleton<factory_registry> archive_factories;

    archive::entry::entry(const mge::path& path,
                          bool is_directory,
                          std::streamsize size,
                          uint32_t index)
        : m_path(path)
        , m_size(size)
        , m_index(index)
        , m_directory(is_directory)
    {}

    archive::entry::entry(const archive::entry& e)
        : m_path(e.m_path)
        , m_size(e.m_size)
        , m_access(e.m_access)
        , m_index(e.m_index)
        , m_directory(e.m_directory)
    {}

    archive::entry::entry(archive::entry&& e)
        : m_path(std::move(e.m_path))
        , m_size(e.m_size)
        , m_access(std::move(e.m_access))
        , m_index(e.m_index)
        , m_directory(e.m_directory)
    {}

    archive::entry& archive::entry::operator=(const archive::entry& e)
    {
        m_path = e.m_path;
        m_size = e.m_size;
        m_access = e.m_access;
        m_index = e.m_index;
        m_directory = e.m_directory;
        return *this;
    }

    archive::entry& archive::entry::operator=(archive::entry&& e)
    {
        m_path = std::move(e.m_path);
        m_size = e.m_size;
        m_access = std::move(e.m_access);
        m_index = e.m_index;
        m_directory = e.m_directory;
        return *this;
    }

    const mge::path& archive::entry::path() const
    {
        return m_path;
    }

    input_stream_ref archive::entry::open()
    {
        return m_access->open(m_index);
    }

    void archive::entry::set_access(const archive_access_ref& a)
    {
        m_access = a;
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
        auto factory = archive_factories->get_factory(m_file);
        if (!factory) {
            MGE_THROW(mge::illegal_state) << "Archive " << m_file << " is not valid";
        }
        m_access = factory->create_archive_access(m_file, m_open_mode);
        m_entries = m_access->entries();
        for(auto& e : m_entries) {
            e.set_access(m_access);
        }
    }

    const archive::entry& archive::find_entry(const path& p) const
    {
        auto it = std::find_if(m_entries.begin(),
                               m_entries.end(),
                               [&](const archive::entry& e) -> bool {
                                   return e.path() == p;
                               });
        if (it == m_entries.end()) {
            MGE_THROW(no_such_element) << "No such archive entry: " << p;
        } else {
            return *it;
        }
    }


}
