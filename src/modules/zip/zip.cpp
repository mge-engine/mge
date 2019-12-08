#include "mge/core/archive_access_factory.hpp"
#include "mge/core/log.hpp"
#include <string>
#include <zip.h>
#include <stdio.h>

// cannot use zip as namespace as that is already defined by zip.h
MGE_DEFINE_LOG(ZIP);

namespace {
    std::ostream& operator <<(std::ostream& os, zip_source_cmd_t c)
    {
        switch (c) {
#define HANDLE_CMD(CMD) case ZIP_SOURCE_ ## CMD: return os << #CMD
        HANDLE_CMD(OPEN);
        HANDLE_CMD(READ);        
        HANDLE_CMD(CLOSE);
        HANDLE_CMD(STAT);
        HANDLE_CMD(ERROR);
        HANDLE_CMD(FREE);
        HANDLE_CMD(SEEK);
        HANDLE_CMD(TELL);
        HANDLE_CMD(BEGIN_WRITE);
        HANDLE_CMD(COMMIT_WRITE);
        HANDLE_CMD(ROLLBACK_WRITE);
        HANDLE_CMD(WRITE);
        HANDLE_CMD(SEEK_WRITE);
        HANDLE_CMD(TELL_WRITE);
        HANDLE_CMD(SUPPORTS);
        HANDLE_CMD(REMOVE);
        HANDLE_CMD(GET_COMPRESSION_FLAGS);
        HANDLE_CMD(BEGIN_WRITE_CLONING);
#undef HANDLE_CMD 
        default:
            return os << "INVALID(" << (int)c << ")";
        }
    }
}


namespace zip_archive {

    class zip_archive_access : 
        public mge::archive_access
    {
    public:
        zip_archive_access(const mge::file &f, mge::open_mode m)
            : mge::archive_access(f, m)
            , m_path_string(f.path().string())
            , m_zip_source(nullptr)
            , m_zip(nullptr)
            , m_source_delete_needed(true)
            , m_entries_refresh_needed(true)
            
        {
            zip_error_init(&m_zip_error);
            m_zip_source = zip_source_function_create(&zip_source_callback, this, &m_zip_error);
            if (m_zip_source == nullptr) {
                MGE_DEBUG_LOG(ZIP) << "Creating zip source function failed";
            } else {
                m_zip = zip_open_from_source(m_zip_source, ZIP_RDONLY, &m_zip_error);
                if (m_zip == nullptr) {
                    MGE_DEBUG_LOG(ZIP) << "Creating zip source failed";
                } else {
                    m_source_delete_needed = false;
                }
            }
        }

        virtual ~zip_archive_access()
        {
            zip_error_fini(&m_zip_error);
            if(m_zip) {
                zip_close(m_zip);
            }

            if (m_zip_source && m_source_delete_needed) {
                zip_source_free(m_zip_source);
            }
        }

        const mge::archive::archive_entries& entries() const override
        {
            if (m_entries_refresh_needed) {
                refresh_entries();
            }
            return m_entries;
        }

    private:
        static bool has_error(zip_error_t* error)
        {
            return zip_error_code_system(error) != 0
                || zip_error_code_zip(error) != 0;
        }

        zip_int64_t cmd_supports()
        {
            return zip_source_make_command_bitmap(ZIP_SOURCE_OPEN,
                                                  ZIP_SOURCE_READ,
                                                  ZIP_SOURCE_CLOSE,
                                                  ZIP_SOURCE_STAT,
                                                  ZIP_SOURCE_ERROR,
                                                  ZIP_SOURCE_FREE,
                                                  ZIP_SOURCE_SEEK,
                                                  ZIP_SOURCE_TELL,
                                                  ZIP_SOURCE_BEGIN_WRITE,
                                                  ZIP_SOURCE_COMMIT_WRITE,
                                                  ZIP_SOURCE_ROLLBACK_WRITE,
                                                  ZIP_SOURCE_WRITE,
                                                  ZIP_SOURCE_SEEK_WRITE,
                                                  ZIP_SOURCE_TELL_WRITE,
                                                  ZIP_SOURCE_SUPPORTS,
                                                  ZIP_SOURCE_REMOVE,
                                                  ZIP_SOURCE_GET_COMPRESSION_FLAGS,
                                                  ZIP_SOURCE_BEGIN_WRITE_CLONING,
                                                  -1);
        }

        zip_int64_t cmd_stat(void *data)
        {
            zip_stat_t *s = reinterpret_cast<zip_stat_t *>(data);
            zip_stat_init(s);
            s->valid = ZIP_STAT_NAME|ZIP_STAT_SIZE;
            s->name = m_path_string.c_str();
            s->size = m_file.size();
            return sizeof(zip_stat_t);
        }

        zip_int64_t cmd_open()
        {
            m_input = m_file.open_for_input();
            return 0;
        }

        zip_int64_t cmd_close()
        {
            m_input.reset();
            return 0;
        }

        zip_int64_t cmd_seek(void *data, zip_uint64_t len)
        {
            zip_error_init(&m_zip_error);
            zip_source_args_seek_t *seek_args = 
                ZIP_SOURCE_GET_ARGS(zip_source_args_seek_t , data, len, &m_zip_error);
            if (has_error(&m_zip_error)) {
                return -1;
            }
            // MGE_DEBUG_LOG(ZIP) << "Seek " << seek_args->whence << " at pos " << seek_args->offset;

            mge::input_stream::direction_type dir = 
                static_cast<mge::input_stream::direction_type>(seek_args->whence);
            return m_input->seek(seek_args->offset, dir);
        }

        zip_int64_t cmd_tell()
        {
            return m_input->position();
        }

        zip_int64_t cmd_read(void *data, zip_uint64_t len)
        {
            return m_input->read(data, len);
        }

        static zip_int64_t zip_source_callback(void * userdata, void * data, zip_uint64_t len, zip_source_cmd_t cmd)
        {
            // MGE_DEBUG_LOG(ZIP) << "CALLBACK " << cmd;
            if (userdata == nullptr) {
                return -1;
            }

            zip_archive_access *self = reinterpret_cast<zip_archive_access *>(userdata);

            switch(cmd) {
            case ZIP_SOURCE_SUPPORTS:
                return self->cmd_supports();
            case ZIP_SOURCE_STAT:
                return self->cmd_stat(data);
            case ZIP_SOURCE_OPEN:
                return self->cmd_open();
            case ZIP_SOURCE_SEEK:
                return self->cmd_seek(data, len);
            case ZIP_SOURCE_TELL:
                return self->cmd_tell();
            case ZIP_SOURCE_READ:
                return self->cmd_read(data, len);
            case ZIP_SOURCE_CLOSE:
                return self->cmd_close();
            case ZIP_SOURCE_FREE:
                return 0;
            default:
                return -1;
            }
        }

        void refresh_entries() const
        {
            auto num_entries = zip_get_num_entries(m_zip, 0);
            for (zip_int64_t i=0; i<num_entries; ++i) {
                zip_stat_t stat = {};
                int rc = zip_stat_index(m_zip, i, 0, &stat);
                if (rc != 0) {
                    break;
                }
                std::string n(stat.name);
                bool is_directory = false;
                if (n.back() == '/' && stat.size == 0) {
                    is_directory = true;
                    n.resize(n.size() - 1);
                }

                m_entries.emplace_back(mge::path(n),
                                       is_directory,
                                       stat.size,
                                       stat.index);
            }
        }

        std::string m_path_string;
        mutable mge::archive::archive_entries m_entries;
        zip_source_t* m_zip_source;
        zip_t* m_zip;
        zip_error_t m_zip_error;
        mge::input_stream_ref m_input;
        bool m_source_delete_needed;
        mutable bool m_entries_refresh_needed;
    };

    class archive_access_factory 
        : public mge::archive_access_factory
    {
    public:
        archive_access_factory() = default;
        ~archive_access_factory() = default;

        bool handles_path(const mge::path& p) const override
        {
            MGE_DEBUG_LOG(ZIP) << "Check " << p << " for being a zip file";
            return p.extension().string() == ".zip";
        }

        mge::archive_access_ref create_archive_access(const mge::file& f, 
                                                      mge::open_mode m) override
        {
            MGE_DEBUG_LOG(ZIP) << "Opening " << f << " with mode " << m;

            mge::archive_access_ref result = 
                std::make_shared<zip_archive_access>(f, m);
            return result;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(archive_access_factory, 
        mge::archive_access_factory, 
        zip);
}