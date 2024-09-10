#include "python_context.hpp"
#include "gil_lock.hpp"
#include "pyobject_ref.hpp"
#include "python_engine.hpp"
#include "python_error.hpp"
#include "python_module.hpp"

#include "mge/core/trace.hpp"
#include "mge/script/module.hpp"
#include "mge/script/module_data.hpp"
#include "mge/script/type_data.hpp"
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

        if (!m_engine->interpreter_initialized()) {
            m_engine->initialize_interpreter();
        }

        pyobject_ref main_module(PyImport_AddModule("__main__"));
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

    void interpreter_lost()
    {
        MGE_DEBUG_TRACE(PYTHON) << "Py_AtExit handler, intepreter lost";
        python_engine::interpreter_lost();
    }

    int python_context::main(int argc, const char** argv)
    {
        int rc = 0;
        {
            PyGILState_STATE gstate;
            gstate = PyGILState_Ensure();
            Py_AtExit(interpreter_lost);
            rc = Py_BytesMain(argc, const_cast<char**>(argv));
            // no release as interpreter thread state already lost
        }
        MGE_DEBUG_TRACE(PYTHON) << "Python main returned " << rc;
        return rc;
    }

    bool python_context::is_builtin(const mge::script::type_data_ref& t) const
    {
        return mge::script::dependency::builtin_dependencies().find(t) !=
               mge::script::dependency::builtin_dependencies().end();
    }

    void python_context::bind()
    {
        mge::script::module root = mge::script::module::root();

        bind_helper_module();
        for (const auto& m : root.data()->modules()) {
            bind_module(m);
        }
#if 0
        for (const auto& t : mge::script::type_data::all()) {
            MGE_DEBUG_TRACE(PYTHON) << "Creating type " << t->name();
            if (is_builtin(t)) {
                MGE_DEBUG_TRACE(PYTHON)
                    << "Type " << t->name() << " is builtin";
                continue;
            }
            python_type_ref pt = std::make_shared<python_type>(*this, t);
            m_types[t] = pt;
        }
#endif
    }

    void python_context::bind_module(const mge::script::module_data_ref& data)
    {
        m_modules[data->full_name()] =
            std::make_shared<python_module>(*this, data);
        for (const auto& m : data->modules()) {
            bind_module(m);
        }
    }

    void python_context::bind_helper_module()
    {
        m_modules["__mge__"] =
            std::make_shared<python_module>(*this, "__mge__");
    }

    void python_context::on_interpreter_loss()
    {
        for (auto& m : m_modules) {
            m.second->on_interpreter_loss();
        }
    }

    void python_context::on_interpreter_restore()
    {
        for (auto& m : m_modules) {
            m.second->on_interpreter_restore();
        }
    }

} // namespace mge::python