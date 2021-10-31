#include "mge/script/script_module.hpp"
namespace mge::script {

    void bind_pod_types()
    {
        auto root_module = module::root();

        root_module->type<void>();

        root_module->type<bool>();

        root_module->type<unsigned char>();
        root_module->type<signed char>();
        root_module->type<char>();

        root_module->type<unsigned short>();
        root_module->type<signed short>();
        root_module->type<short>();

        root_module->type<unsigned int>();
        root_module->type<signed int>();
        root_module->type<int>();

        root_module->type<unsigned long>();
        root_module->type<signed long>();
        root_module->type<long>();

        root_module->type<unsigned long long>();
        root_module->type<signed long long>();
        root_module->type<long long>();
    }
} // namespace mge::script