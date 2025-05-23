// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/buffer.hpp"
#include "mge/core/callback_map.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"

#include <istream>

namespace mge {

    MGE_DECLARE_REF(std_istream);
    MGE_DECLARE_REF(input_stream);

    /**
     * An input stream to read bytes.
     */
    class MGECORE_EXPORT input_stream
    {
    private:
        using read_listener_map = callback_map<std::streamsize>;

    public:
        using read_listener = read_listener_map::value_type;

        /**
         * Type of positions and offsets.
         */
        using offset_type = std::streamoff;

        /**
         * Type of number of bytes read.
         */
        using streamsize_type = std::streamsize;

        /// Seek direction type.
        enum direction_type
        {
            POS_BEG = 0, //!< seek from beginning
            POS_CUR = 1, //!< seek from current position
            POS_END = 2  //!< seek from end
        };

        /**
         * Constructor.
         */
        input_stream() = default;

        /**
         * Destructor.
         */
        virtual ~input_stream() = default;

        /**
         * Read one byte from the input stream. The default
         * implementation uses the <code>read(void*,int)</code>
         * method to obtain the data.
         * @return byte that has been read, or -1 at EOF
         * @throw IOException if an I/O error occurs.
         */
        virtual streamsize_type read();

        /**
         * Read more bytes from the input stream. The default
         * implementation is always at EOF.
         * @param destination target buffer
         * @param size        target buffer size
         * @return number of bytes read, 0 on EOF
         */
        streamsize_type read(void* destination, streamsize_type size);

        /**
         * Read the stream data fully into the buffer.
         * @param b buffer to read into
         */
        void read(buffer& b);

        /**
         * Get current position.
         * @return current position, -1 if not known
         */
        virtual offset_type position();

        /** Get current position.
         * This is a convenience method that calls <tt>position()</tt>.
         * @return current position, -1 if not known
         */
        inline offset_type tell()
        {
            return position();
        }

        /**
         * Seek relative to a specific position.
         * @param offset seek offset
         * @param dir    seek direction
         * @return position after seek, -1 if not known
         */
        virtual offset_type seek(offset_type    offset,
                                 direction_type dir = POS_BEG);

        /**
         * @brief Rewind to beginning.
         *
         * @return true if rewind succeded, false if rewinding isn't supported
         */
        inline bool rewind()
        {
            return seek(0, POS_BEG) != -1;
        }

        /**
         * Access the stream as an @c std::istream
         * @return a @c std::istream using this stream's data
         */
        std::istream& istream();

        /**
         * Return whether stream is at end-of-file.
         * @return @c true if stream is at eof
         */
        bool eof();

        /**
         * Add a read listener.
         * @param l listener
         * @return listener key
         */
        unsigned int add_read_listener(const read_listener& l);

        /**
         * Remove read listener.
         * @param key listener key
         */
        void remove_read_listener(unsigned int key);

    protected:
        virtual streamsize_type on_read(void*           destination,
                                        streamsize_type size) = 0;

    private:
        void notify_read(streamsize_type read_bytes);

        std_istream_ref   m_stdstream;
        read_listener_map m_read_listeners;
    };
} // namespace mge
