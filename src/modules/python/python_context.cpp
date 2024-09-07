#include "python_context.hpp"
#include "bind_step.hpp"
#include "gil_lock.hpp"
#include "pyobject_ref.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"

#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_data.hpp"

#include <algorithm>
#include <mutex>

namespace mge {
    MGE_USE_TRACE(PYTHON);
}

namespace mge::python {

    python_context::python_context(const python_engine_ref& engine)
        : m_engine(engine)
    {}

    python_context::~python_context() {}

    void python_context::eval(const std::string& code)
    {
        std::lock_guard<gil_lock> lock(gil_lock::instance());
        pyobject_ref              main_module(PyImport_AddModule("__main__"));
        error::check_error();
        if (!main_module) {
            MGE_THROW(python::error) << "Cannot get main module";
        }
        pyobject_ref main_dict(PyModule_GetDict(main_module.get()));
        error::check_error();
        if (!main_dict) {
            MGE_THROW(python::error) << "Cannot get main module dictionary";
        }
        pyobject_ref result(PyRun_String(code.c_str(),
                                         Py_file_input,
                                         main_dict.get(),
                                         main_dict.get()));
        error::check_error();
    }

    int python_context::main(int argc, const char** argv)
    {
        int rc = Py_BytesMain(argc, const_cast<char**>(argv));
        MGE_DEBUG_TRACE(PYTHON) << "Python main returned " << rc;
        return rc;
    }

    void python_context::create_steps_for_module(
        const mge::script::module_data_ref& module,
        std::vector<bind_step_ref>&         steps)
    {
        bind_step_ref step = std::make_shared<bind_step_module>(module);
        steps.push_back(step);
        MGE_DEBUG_TRACE(PYTHON) << "Created step for " << step->provides();
        for (const auto& f : module->functions()) {
            bind_step_ref fstep =
                std::make_shared<bind_step_function>(module, f);
            steps.push_back(fstep);
            MGE_DEBUG_TRACE(PYTHON) << "Created step for " << fstep->provides();
        }
        for (const auto& t : module->types()) {
            bind_step_ref tstep = std::make_shared<bind_step_type>(module, t);
            steps.push_back(tstep);
            MGE_DEBUG_TRACE(PYTHON) << "Created step for " << tstep->provides();
        }
        for (const auto& m : module->modules()) {
            create_steps_for_module(m, steps);
        }
    }

    void python_context::bind()
    {
        mge::script::module root = mge::script::module::root();

        std::vector<bind_step_ref> steps;

        for (const auto& m : root.data()->modules()) {
            create_steps_for_module(m, steps);
        }

        mge::script::dependency_set fulfilled_dependencies(
            mge::script::dependency::builtin_dependencies());
        std::vector<bind_step_ref> sorted_steps;

        MGE_DEBUG_TRACE(PYTHON) << "Builtin dependencies: ";
        for (const auto& dep : fulfilled_dependencies) {
            MGE_DEBUG_TRACE(PYTHON) << "  " << dep;
        }
        bool     made_progress = true;
        uint32_t iteration = 0;
        while (made_progress) {
            ++iteration;
            MGE_DEBUG_TRACE(PYTHON)
                << "Sorting steps, iteration #" << iteration;
            made_progress = false;
            auto it = steps.begin();
            while (it != steps.end()) {
                const auto& step = *it;
                MGE_DEBUG_TRACE(PYTHON)
                    << "Processing step for " << step->provides();
                if (fulfilled_dependencies.find(step->provides()) !=
                    fulfilled_dependencies.end()) {
                    MGE_DEBUG_TRACE(PYTHON) << "Step already fulfilled";
                    continue;
                }
                auto deps = step->dependencies();
                MGE_DEBUG_TRACE(PYTHON) << "Dependencies: ";
                for (const auto& dep : deps) {
                    MGE_DEBUG_TRACE(PYTHON) << "  " << dep;
                }
                if (std::all_of(deps.begin(),
                                deps.end(),
                                [&fulfilled_dependencies](const auto& dep) {
                                    return fulfilled_dependencies.find(dep) !=
                                           fulfilled_dependencies.end();
                                })) {
                    MGE_DEBUG_TRACE(PYTHON) << "All dependencies fulfilled";
                    fulfilled_dependencies.insert(step->provides());
                    sorted_steps.push_back(step);
                    it = steps.erase(it);
                    made_progress = true;
                } else {
                    MGE_DEBUG_TRACE(PYTHON) << "Dependencies not fulfilled for "
                                            << step->provides();
                    ++it;
                }
            }
        }
    }
} // namespace mge::python