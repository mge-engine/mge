#pragma once
#include <ffi.h>

namespace mge {

    class closure
    {
    public:
        closure();
        ~closure();

    private:
        void* m_writable_address{0};
        void* m_executable_address{0};
    };

} // namespace mge