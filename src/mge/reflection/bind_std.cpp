#include <cstdint>
#include "mge/reflection/type.hpp"

#include <string>

namespace mge {
    namespace reflection {
        void bind_std()
        {
            type::get<std::string>()
                .constructor()
                .destructor()
                .method("size", &std::string::size);
        }
    }
}
