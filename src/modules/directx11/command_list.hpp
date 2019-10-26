// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "render_context.hpp"
#include "mge/graphics/command_list.hpp"
#include "win32/com_unique_ptr.hpp"

namespace dx11 {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        virtual void clear(const mge::rgba_color& c) override;
        virtual void clear_depth(float value) override;
        virtual void clear_stencil(int32_t value) override;

        ID3D11CommandList *native_command_list() const { return m_command_list.get(); }
    protected:
        void on_finish() override;
        void on_set_pipeline(const mge::pipeline_ref& pipeline);
    private:
        mge::com_unique_ptr<ID3D11DeviceContext> m_deferred_context;
        mge::com_unique_ptr<ID3D11CommandList>   m_command_list;
    };
}
