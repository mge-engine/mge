// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/mutex.hpp"
#include "mge/core/singleton.hpp"

#include "python_fwd.hpp"

#include <string>

namespace mge::python {

    class python_interpreter
    {
    public:
        python_interpreter();
        ~python_interpreter();

        static python_interpreter& instance()
        {
            return *s_interpreter;
        }

        python_context& context() const
        {
            return *m_context;
        }

        const python_context_ref& context_ref() const
        {
            return m_context;
        }

    private:
        mge::mutex         m_mutex{"python_interpreter"};
        std::wstring       m_home;
        python_context_ref m_context;
        bool               m_initialized{false};

        static mge::singleton<python_interpreter> s_interpreter;
    };

} // namespace mge::python