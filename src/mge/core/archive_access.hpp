#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/open_mode.hpp"
#include "mge/core/file.hpp"
#include "mge/core/gist.hpp"

#include <iosfwd>

namespace mge {
    
    MGE_DECLARE_REF(archive_access);

    /**
     * @brief Provide access to an archive.
     */
    class MGE_CORE_EXPORT archive_access : public noncopyable
    {
    public:
        /**
         * @brief Construct a new archive access object.
         * 
         * @param f     file to use as archive
         * @param m     open mode
         */
        archive_access(const file& f, open_mode m);
        virtual ~archive_access() = default;

        void gist(std::ostream& os);
    protected:
        file      m_file;
        open_mode m_open_mode;
    };
}