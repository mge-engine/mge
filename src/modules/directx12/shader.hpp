// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/win32/com_unique_ptr.hpp"

#include <string>
#include <vector>

namespace mge::dx12 {
    class render_context;
    class shader : public mge::shader
    {
    public:
        shader(render_context& context, shader_type type);
        ~shader();

        const D3D12_INPUT_ELEMENT_DESC* input_layout() const
        {
            return m_input_layout.data();
        }

        uint32_t input_layout_count() const
        {
            return static_cast<uint32_t>(m_input_layout.size());
        }

    protected:
        virtual void on_compile(std::string_view code) override;
        virtual void on_set_code(const mge::buffer& code) override;

    public:
        ID3DBlob* code() const
        {
            return m_code.get();
        }

        void reflect(
            mge::program::attribute_list&              attributes,
            mge::program::uniform_list&                uniforms,
            mge::program::uniform_block_metadata_list& uniform_buffers) const;

    private:
        std::string profile() const;
        void        create_input_layout();

        mge::com_unique_ptr<ID3DBlob>         m_code;
        std::vector<D3D12_INPUT_ELEMENT_DESC> m_input_layout;
        std::vector<std::string>              m_semantic_names;
    };

    inline shader& dx12_shader(mge::shader& s)
    {
        return static_cast<shader&>(s);
    }

    inline const shader& dx12_shader(const mge::shader& s)
    {
        return static_cast<const shader&>(s);
    }

} // namespace mge::dx12