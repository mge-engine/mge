#include "mge/application/update_thread.hpp"
#include "mge/core/crash.hpp"
#include "mge/application/application.hpp"

namespace mge {

    volatile update_thread *
    update_thread::s_update_thread;

    update_thread::update_thread()
    {
        if(s_update_thread != nullptr) {
            MGE_THROW(mge::illegal_state(),
                      "Update thread is supposed to be a singleton");
        }
        s_update_thread = this;
    }

    update_thread::~update_thread()
    {
        if(s_update_thread != nullptr) {
            crash("Update thread still active on destruction");
        }
    }

    void
    update_thread::run()
    {
        try {
            s_update_thread = this;
            s_update_thread = nullptr;
        } catch(...) {
            s_update_thread = nullptr;
            throw;
        }
    }
}
