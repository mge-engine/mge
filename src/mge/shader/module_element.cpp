#include "mge/shader/module_element.hpp"
#include "mge/shader/program_element.hpp"
namespace mge {
namespace shader {
    module_element::module_element(mge::shader::module* m,
                                   uint32_t begin,
                                   uint32_t end)
        :program_element(m->program())
        ,m_location(m, begin, end)
    {}

    module_element::module_element(mge::shader::module* m)
        :program_element(m->program())
        ,m_location(m)
    {}

    module_element::module_element(mge::shader::program* p)
        :program_element(p)
    {}

}
}
