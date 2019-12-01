#include "mge/core/archive_access_factory.hpp"
#include <zip.h>
// cannot use zip as namespace as that is already defined by zip.h
namespace zip_archive {

    class zip_archive_access : 
        public mge::archive_access
    {
    public:
        zip_archive_access(const mge::file &f, mge::open_mode m)
            : mge::archive_access(f, m)
            , m_zip_source(nullptr)
            , m_source_delete_needed(true)
        {
            zip_error_t err = {};
            m_zip_source = zip_source_function_create(&zip_source_callback, this, &err);
            if (m_zip_source == nullptr) {

            }
        }

        virtual ~zip_archive_access()
        {
            if (m_zip_source && m_source_delete_needed) {
                zip_source_free(m_zip_source);
            }
        }

    private:
        static zip_int64_t zip_source_callback(void * arg1, void * arg2, zip_uint64_t arg3, zip_source_cmd_t cmd)
        {
            return -1;
        }

        zip_source_t* m_zip_source;
        bool          m_source_delete_needed;
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