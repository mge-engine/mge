// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/callback_map.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/memory.hpp"

#include <ostream>

namespace mge {

    MGE_DECLARE_REF(std_ostream);
    MGE_DECLARE_REF(output_stream);

    /**
     * An output stream to write bytes.
     */
    class MGECORE_EXPORT output_stream
    {
    public:
        /**
         * Type of number of bytes read.
         */
        using streamsize_type = std::streamsize;

    private:
        using write_listener_map = callback_map<streamsize_type>;

    public:
        using write_listener = write_listener_map::value_type;

        /**
         * Type of positions and offsets.
         */
        using offset_type = std::streamoff;

        /**
         * Constructor.
         */
        output_stream() = default;

        /**
         * Destructor.
         */
        virtual ~output_stream() = default;

        /**
         * Write one byte to the stream.
         * @throws mge::io_error if an I/O error occurs.
         */
        void write(uint8_t byte);

        /**
         * Write number of bytes to the stream
         * implementation is always at EOF.
         * @param buffer buffer to write
         * @param size   number of bytes to write
         */
        void write(const void* buffer, streamsize_type size);

        /**
         * Access the stream as an @c std::ostream
         * @return a @c std::ostream using this stream's data
         */
        std::ostream& ostream();

        /**
         * Add a write listener.
         * @param l listener
         * @return listener key
         */
        uint32_t add_write_listener(const write_listener& l);

        /**
         * Remove write listener.
         * @param key listener key
         */
        void remove_write_listener(uint32_t key);

        /**
         * Enforces stream flush (i.e. force the streams data to
         * be really written).
         */
        void flush();

    protected:
        virtual void on_write(const void* buffer, streamsize_type size) = 0;
        virtual void on_flush() = 0;

    private:
        void notify_write(streamsize_type written_bytes);

        std_ostream_ref    m_stdstream;
        write_listener_map m_write_listeners;
    };
} // namespace mge
