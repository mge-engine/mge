// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/ui/dllexport.hpp"
#include "mge/ui/ui_fwd.hpp"
namespace mge {
    namespace ui {

        class context_object : public noncopyable
        {
        public:
            context_object(mge::ui::context &ctx);
            ~context_object() = default;
            mge::ui::context &      context();
            const mge::ui::context &context() const;

        private:
            mge::ui::context_ref m_context;
        };
    } // namespace ui
} // namespace mge
