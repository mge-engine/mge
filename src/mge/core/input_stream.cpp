// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/input_stream.hpp"
#include "mge/core/trace.hpp"
#include <cstring>

#define MGE_STREAMBUF_BUFFER_SIZE 1024

MGE_USE_TRACE(MGE);

namespace mge {

    class input_stream_buffer : public std::streambuf
    {
    public:
        input_stream_buffer()
            : m_stream(0)
            , m_buffer(0)
            , m_buffer_size(0)
            , m_buffer_used(0)
        {}

        input_stream_buffer(input_stream* stream)
            : m_stream(stream)
            , m_buffer(0)
            , m_buffer_size(0)
            , m_buffer_used(0)
        {
            if (m_stream) {
                m_buffer = new char[MGE_STREAMBUF_BUFFER_SIZE];
                m_buffer_size = MGE_STREAMBUF_BUFFER_SIZE;
                setg(m_buffer, m_buffer, m_buffer);
            }
        }

        input_stream_buffer(const input_stream_buffer& buffer)
            : std::streambuf()
            , m_stream(buffer.m_stream)
            , m_buffer(0)
            , m_buffer_size(0)
            , m_buffer_used(0)
        {
            if (m_stream) {
                m_buffer_size = buffer.m_buffer_size;
                m_buffer = new char[buffer.m_buffer_size];
                memcpy(m_buffer, buffer.m_buffer, buffer.m_buffer_used);
                setg(m_buffer,
                     m_buffer + (buffer.gptr() - buffer.eback()),
                     m_buffer + m_buffer_used);
            }
        }

        virtual ~input_stream_buffer()
        {
            if (m_buffer) {
                delete[] m_buffer;
            }
        }

        input_stream_buffer& operator=(const input_stream_buffer& buffer)
        {
            if (this != &buffer) {
                m_stream = buffer.m_stream;
                delete[] m_buffer;
                m_buffer = 0;
                m_buffer_size = 0;
                m_buffer_used = 0;
                setg(0, 0, 0);
                if (m_stream) {
                    m_buffer = new char[buffer.m_buffer_size];
                    m_buffer_size = buffer.m_buffer_size;
                    m_buffer_used = buffer.m_buffer_used;
                    memcpy(m_buffer, buffer.m_buffer, m_buffer_used);
                    setg(m_buffer,
                         m_buffer + (buffer.gptr() - buffer.eback()),
                         m_buffer + m_buffer_used);
                }
            }
            return *this;
        }

        virtual std::streambuf::pos_type seekoff(std::streamoff         offset,
                                                 std::ios_base::seekdir way,
                                                 std::ios_base::openmode)
        {
            input_stream::offset_type pos = -1;

            if (way == std::ios_base::beg) {
                pos = m_stream->seek(offset, input_stream::POS_BEG);
            } else if (way == std::ios_base::end) {
                pos = m_stream->seek(offset, input_stream::POS_END);
            } else {
                pos = m_stream->seek(offset, input_stream::POS_CUR);
            }

            if (pos != -1) {
                m_buffer_used = 0;
                setg(m_buffer, m_buffer, m_buffer);
                if (way != std::ios_base::end) {
                    underflow();
                }
            }

            return pos;
        }

        virtual std::streambuf::int_type underflow()
        {
            if (gptr() < egptr()) {
                return *(gptr());
            } else {
                // MGE_DEBUG_LOG(MGE) << "underflow stream";
                input_stream::streamsize_type bytes_read = m_stream->read(
                    m_buffer,
                    static_cast<input_stream::streamsize_type>(m_buffer_size));
                // MGE_DEBUG_LOG(MGE) << "underflow: bytes read: " <<
                // bytes_read;
                if (bytes_read == -1) {
                    // MGE_DEBUG_LOG(MGE) << "underflow: eof";
                    setg(0, 0, 0);
                    return traits_type::eof();
                } else {
                    // MGE_DEBUG_LOG(MGE) << "underflow: set buffer";
                    m_buffer_used = (size_t)bytes_read;
                    setg(m_buffer, m_buffer, m_buffer + m_buffer_used);
                    return *(gptr());
                }
            }
        }

    private:
        input_stream* m_stream;
        char*         m_buffer;
        size_t        m_buffer_size;
        size_t        m_buffer_used;
    };

    class std_istream : public std::istream, public noncopyable
    {
    public:
        std_istream(input_stream* inputstream)
            : std::istream(&m_buffer)
            , m_buffer(inputstream)
        {}

        ~std_istream() = default;

    private:
        input_stream_buffer m_buffer;
    };

    std::istream& input_stream::istream()
    {
        if (!m_stdstream) {
            m_stdstream = std::make_shared<std_istream>(this);
        }

        return *m_stdstream;
    }

    input_stream::streamsize_type input_stream::read()
    {
        unsigned char c;
        if (read(&c, 1) == 1) {
            return (streamsize_type)c;
        } else {
            return -1;
        }
    }

    input_stream::streamsize_type
    input_stream::read(void* destination, input_stream::streamsize_type size)
    {
        auto result = on_read(destination, size);
        if (result) {
            notify_read(result);
        }
        return result;
    }

    bool input_stream::eof()
    {
        char c;
        auto r = read(&c, 0);
        return r == -1;
    }

    input_stream::offset_type input_stream::position() { return -1; }

    input_stream::offset_type input_stream::seek(input_stream::offset_type,
                                                 direction_type)
    {
        return -1;
    }

    unsigned int
    input_stream::add_read_listener(const input_stream::read_listener& l)
    {
        return m_read_listeners.insert(l);
    }

    void input_stream::remove_read_listener(unsigned int key)
    {
        m_read_listeners.erase(key);
    }

    void input_stream::notify_read(input_stream::streamsize_type read_bytes)
    {
        m_read_listeners(read_bytes);
    }
} // namespace mge
