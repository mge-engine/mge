#include "mge/graphics/drawable.hpp"

namespace mge {

    drawable::drawable()
    {}

    drawable::~drawable()
    {}

    bool
    drawable::prepared() const
    {
        return true;
    }

    void
    drawable::prepare(render_context& context)
    {}
}