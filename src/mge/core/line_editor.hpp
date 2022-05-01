#pragma once
#include "mge/core/dllexport.hpp"

#include <atomic>
#include <string>

namespace mge {

    class MGECORE_EXPORT line_editor
    {
    public:
        line_editor();
        ~line_editor();

        std::string line();

    private:
        static std::atomic<int> s_instances;
    };

} // namespace mge