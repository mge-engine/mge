#include "python_context.hpp"
#include "bind_step.hpp"
#include "gil_lock.hpp"
#include "pyobject_ref.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"

#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_data.hpp"
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

    void python_context::bind()
    {
        mge::script::module root = mge::script::module::root();

        std::vector<bind_step_ref> steps;

        for (const auto& m : root.data()->modules()) {
            steps.emplace_back(std::make_shared<bind_step_module>(m));
            for (const auto& f : m->functions()) {
                steps.emplace_back(std::make_shared<bind_step_function>(m, f));
            }
        }
    }

} // namespace mge::python