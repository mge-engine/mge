#include "mge/core/archive_access_factory.hpp"

namespace zip {

    class zip_archive_access : 
        public mge::archive_access
    {
    public:
        zip_archive_access(const mge::file &f, mge::open_mode m)
            :mge::archive_access(f, m)
        {}

        virtual ~zip_archive_access()
        {}
    };

    class archive_access_factory 
        : public mge::archive_access_factory
    {
    public:
        archive_access_factory() = default;
        ~archive_access_factory() = default;
        mge::archive_access_ref create_archive_access(const mge::file& f, 
                                                      mge::open_mode m) override
        {
            mge::archive_access_ref result = 
                std::make_shared<zip_archive_access>(f, m);
            return result;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(archive_access_factory, 
        mge::archive_access_factory, 
        zip);
}