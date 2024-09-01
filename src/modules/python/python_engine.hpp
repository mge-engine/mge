#pragma once
#include "mge/config.hpp"
#include "mge/script/script_engine.hpp"
#include "python.hpp"

#include <atomic>
#include <string>
namespace mge::python {

    class python_engine : public script_engine
    {
    public:
        python_engine();
        ~python_engine() override;

        mge::script_context_ref create_context() override;

    private:
#ifdef MGE_OS_WINDOWS
        static void  function_with_address_in_this_module();
        std::wstring compute_python_home();
#else
#    error Missing port
#endif
        void initialize_interpreter();
        void finalize_interpreter();

        static std::atomic<uint64_t> s_initialized_engines;
    };
} // namespace mge::python