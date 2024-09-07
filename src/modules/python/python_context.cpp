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
        MGE_DEBUG_TRACE(PYTHON)
            << "Created step for module: " << step->provides();
        for (const auto& f : module->functions()) {
            bind_step_ref fstep =
                std::make_shared<bind_step_function>(module, f);
            steps.push_back(fstep);
            MGE_DEBUG_TRACE(PYTHON)
                << "Created step for function: " << fstep->provides();
        }
        for (const auto& t : module->types()) {
            bind_step_ref tstep = std::make_shared<bind_step_type>(module, t);
            steps.push_back(tstep);
            MGE_DEBUG_TRACE(PYTHON)
                << "Created step for type: " << tstep->provides();
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
        while (!steps.empty()) {
            bool found = false;
            for (auto it = steps.begin(); it != steps.end();) {
                const auto& deps = (*it)->dependencies();
                bool        deps_included =
                    std::includes(deps.begin(),
                                  deps.end(),
                                  fulfilled_dependencies.begin(),
                                  fulfilled_dependencies.end());

                if (deps_included) {
                    sorted_steps.push_back(*it);
                    fulfilled_dependencies.insert((*it)->provides());
                    MGE_DEBUG_TRACE(PYTHON)
                        << "Resolved step: " << (*it)->provides();
                    it = steps.erase(it);
                    found = true;
                } else {
                    ++it;
                }
            }
            if (!found) {
                break;
            }
        }
        MGE_DEBUG_TRACE(PYTHON) << "Resolved steps: " << sorted_steps.size();
        MGE_DEBUG_TRACE(PYTHON) << "Unresolved steps: " << steps.size();

        for (const auto& step : steps) {
            MGE_DEBUG_TRACE(PYTHON) << "Unresolved step: " << step->provides();
            for (const auto& dep : step->dependencies()) {
                MGE_DEBUG_TRACE(PYTHON) << "  depends on: " << dep;
            }
        }
    }

} // namespace mge::python