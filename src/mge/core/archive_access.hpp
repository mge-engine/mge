// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/archive.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/file.hpp"
#include "mge/core/gist.hpp"
#include "mge/core/open_mode.hpp"
#include "mge/core/types.hpp"

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
        archive_access(const file &f, open_mode m);
        /**
         * @brief Destructor.
         */
        virtual ~archive_access() = default;

        /**
         * @brief Gist helper method.
         * @param os output stream
         */
        void gist(std::ostream &os);

        /**
         * Read the entries from the archive.
         * @return list of archive entries
         */
        virtual const archive::archive_entries &entries() const = 0;
        /**
         * @brief Open for reading.
         *
         * @param index index in archive
         * @return reference to input stream
         */
        virtual input_stream_ref open(uint64_t index) = 0;

    protected:
        file      m_file;
        open_mode m_open_mode;
    };
} // namespace mge
