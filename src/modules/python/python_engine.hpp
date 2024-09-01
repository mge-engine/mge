#pragma once
#include "mge/script/script_engine.hpp"
#include "python.hpp"

#include <atomic>
namespace mge::python {

    class python_engine : public script_engine
    {
    public:
        python_engine();
        ~python_engine() override;

        mge::script_context_ref create_context() override;

    private:
        void initialize_interpreter();
        void finalize_interpreter();

        static std::atomic<uint64_t> s_initialized_engines;
    };
} // namespace mge::python