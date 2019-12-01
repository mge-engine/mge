#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/archive_access.hpp"
#include "mge/core/component.hpp"
#include "mge/core/file.hpp"
namespace mge {

    MGE_DECLARE_REF(archive_access_factory);

    /**
     * @brief Factory to create archive access.
     * 
     * One factory class handles one kind of archive, such as 
     * "zip" and can create an archive access object that can be 
     * used to read from the archive or to write to it.
     */
    class MGE_CORE_EXPORT archive_access_factory
        : public component<archive_access_factory>
    {
    public:
        /**
         * @brief Construct a new archive access factory object.
         */
        archive_access_factory() = default;
        /**
         * @brief Destroy the archive access factory object.
         */
        virtual ~archive_access_factory() = default;
        /**
         * @brief Get whether the access factory handles given path.
         * 
         * @param p path to check
         * @return true if access factory can create archive access for the path
         * @return false if access factory cannot be used for archive access
         */
        virtual bool handles_path(const path& p) const = 0;

        /**
         * @brief Create a archive access object.
         * 
         * @param f     archive file name
         * @param m     access mode
         * 
         * @return Create access object.
         */
        virtual archive_access_ref create_archive_access(const file& f, open_mode m) = 0;
    };
}