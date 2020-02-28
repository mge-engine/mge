#include "mge/shader/statement.hpp"

namespace mge {
namespace shader {

    statement::statement(module *m)
        :module_element(m)
    {}

    statement::statement(module *m, uint32_t begin, uint32_t end)
        :module_element(m, begin, end)
    {}

}
}
