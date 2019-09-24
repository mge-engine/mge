#include "library.hpp"

namespace vk {
    
    library::library()
    {}

    library::~library()
    {}

    static library_ref s_library;

    const library_ref& 
    library::instance()
    {
        if(!s_library) {
            s_library = std::make_shared<library>();
        }
        return s_library;
    }
}