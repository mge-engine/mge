#include "mge/core/configuration.hpp"
#include "mge/core/module.hpp"

int main(int argc, const char** argv)
{

    if (!mge::configuration::loaded()) {
        mge::configuration::load();
    }
    mge::module::load_all();

    return 0;
}