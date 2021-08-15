// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/output_stream.hpp"
#include "mge/core/buffer.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    class output_stream_buffer : public std::streambuf
    {
    public:
        output_stream_buffer(output_stream* outputstream, size_t size)
            : m_stream(outputstream)
            , m_buffer(size)
        {
            if (size == 0) {
                MGE_THROW(mge::illegal_argument)
                    << "Argument 'size' must be > 0";
            }
            auto* s = reinterpret_cast<char*>(m_buffer.data());
            setp(s, s + size - 1);
        }

        output_stream_buffer(const output_stream_buffer& buffer)
            : m_stream(buffer.m_stream)
            , m_buffer(buffer.m_buffer.size())
        {
            auto* s = reinterpret_cast<char*>(m_buffer.data());
            setp(s, s + m_buffer.size() - 1);
        }

        virtual ~output_stream_buffer()
        {
            // we cannot do sync here, as output stream buffer is
            // destroyed after a elements of derived stream
            // already have been destroyed
        }

        std::streambuf::int_type sync() override
        {
            auto* s = reinterpret_cast<char*>(m_buffer.data());
            auto* e = pptr();
            m_stream->write(reinterpret_cast<const void*>(s), e - s);
            m_stream->flush();
            setp(s, s + m_buffer.size() - 1);
            return 0;
        }

        std::streambuf::int_type overflow(int_type c) override
        {
            if (c == traits_type::eof()) {
                sync();
            } else {
                *m_buffer.rbegin() = static_cast<uint8_t>(c);
                auto* s = reinterpret_cast<char*>(m_buffer.data());
                m_stream->write(s, m_buffer.size());
                setp(s, s + m_buffer.size() - 1);
            }
            return ~traits_type::eof();
        }

    private:
        output_stream* m_stream;
        mge::buffer    m_buffer;
    };

    class std_ostream : public std::ostream, public noncopyable
    {
    public:
        static constexpr size_t BUFFER_SIZE = 4096;

        std_ostream(output_stream* outputstream)
            : std::ostream(&m_buffer)
            , m_buffer(outputstream, BUFFER_SIZE)
        {}

        ~std_ostream() = default;

    private:
        output_stream_buffer m_buffer;
    };

    void output_stream::write(uint8_t byte) { write(&byte, 1); }

    void output_stream::write(const void*                    buffer,
                              output_stream::streamsize_type size)
    {
        if (size == 0) {
            return;
        }
        if (buffer == nullptr) {
            MGE_THROW(mge::illegal_argument)
                << "Argument 'buffer' must not be null";
        }
        on_write(buffer, size);
        notify_write(size);
    }

    void output_stream::flush() { on_flush(); }

    std::ostream& output_stream::ostream()
    {
        if (!m_stdstream) {
            m_stdstream = std::make_shared<std_ostream>(this);
        }
        return *m_stdstream;
    }

    unsigned int
    output_stream::add_write_listener(const output_stream::write_listener& l)
    {
        return m_write_listeners.insert(l);
    }

    void output_stream::remove_write_listener(unsigned int key)
    {
        m_write_listeners.erase(key);
    }

    void output_stream::notify_write(streamsize_type written_bytes)
    {
        m_write_listeners(written_bytes);
    }

} // namespace mge