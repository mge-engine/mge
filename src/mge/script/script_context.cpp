// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/script_context.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/trace.hpp"
#include "mge/script/script_binder.hpp"
#include <map>

namespace mge {

    MGE_USE_TRACE(SCRIPT);

    class binder
    {
    public:
        binder() = default;
        ~binder() = default;

        void resolve_bindings()
        {
            MGE_DEBUG_TRACE(SCRIPT) << "Resolve bindings";
            script_binder::implementations([&](std::string_view s) {
                if (m_processed_binders.find(s) == m_processed_binders.end()) {
                    MGE_DEBUG_TRACE(SCRIPT)
                        << "Need to resolve bindings with '" << s << "'";
                    auto binder = script_binder::create(s);
                    binder->bind();
                    m_processed_binders.emplace(s, binder);
                }
            });
        }

    private:
        std::map<std::string, script_binder_ref, std::less<>>
            m_processed_binders;
    };

    static singleton<binder> s_binder;

    script_context::script_context() { s_binder->resolve_bindings(); }
} // namespace mge