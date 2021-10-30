#include "mge/script/script_module.hpp"
namespace mge::script {

    void bind_pod_types()
    {
        auto root_module = module::root();
        /*
                root_module->type<unsigned char>();
                root_module->type<signed char>();
                root_module->type<char>();
        */
    }
} // namespace mge::script