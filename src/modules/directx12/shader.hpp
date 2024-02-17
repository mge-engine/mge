// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx12 {
    class render_context;
    class shader : public mge::shader
    {
    public:
        shader(render_context& context, shader_type type);
        ~shader();

    protected:
        virtual void on_compile(std::string_view code) override;
        virtual void on_set_code(const mge::buffer& code) override;

    public:
        ID3DBlob* code() const { return m_code.get(); }

        void reflect(mge::program::attribute_list&      attributes,
                     mge::program::uniform_list&        uniforms,
                     mge::program::uniform_buffer_list& uniform_buffers) const;

    private:
        std::string profile() const;

        mge::com_unique_ptr<ID3DBlob> m_code;
    };
} // namespace mge::dx12