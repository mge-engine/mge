#include "mge/graphics/command.hpp"

namespace mge {

    command::command()
    {}

    void
    command::set_pipeline(const pipeline_ref& pipeline)
    {
        m_pipeline = pipeline;
    }

}
