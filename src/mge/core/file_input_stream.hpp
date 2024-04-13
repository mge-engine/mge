#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/core/path.hpp"

namespace mge {

    class MGECORE_EXPORT file_input_stream : public input_stream
    {
    public:
        file_input_stream(const path& path);
        virtual ~file_input_stream();

        offset_type position() override;
        offset_type seek(offset_type offset, direction_type dir) override;;

    private:
        streamsize_type on_read(void*           destination,
                                streamsize_type size) override;

        path  m_path;
        FILE* m_file;
    };

} // namespace mge