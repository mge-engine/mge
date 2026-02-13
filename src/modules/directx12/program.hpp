// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx12.hpp"
#include "dx12_fwd.hpp"
#include "mge/core/to_underlying.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/win32/com_ptr.hpp"
#include "shader.hpp"

#include <array>
#include <unordered_set>

namespace mge::dx12 {

    class program : public mge::program
    {
    public:
        program(render_context& context);
        virtual ~program();

        inline shader* program_shader(mge::shader_type t) const
        {
            return m_shaders[mge::to_underlying(t)];
        }

        ID3D12RootSignature* root_signature() const
        {
            return m_root_signature.Get();
        }

        uint32_t buffer_bind_point(const std::string& name) const;

        bool uses_in_vertex_shader(const std::string& name) const;
        bool uses_in_pixel_shader(const std::string& name) const;

        const uniform_block_metadata_list& uniform_blocks() const
        {
            return m_uniform_block_metadata;
        }

    protected:
        void on_link() override;
        void on_set_shader(mge::shader* shader) override;

    private:
        void collect_information();
        void create_root_signature();

        std::array<shader*,
                   mge::to_underlying(mge::shader_type::MAX_SHADER_TYPE) + 1>
            m_shaders{};
        std::array<std::unordered_set<std::string>,
                   mge::to_underlying(mge::shader_type::MAX_SHADER_TYPE) + 1>
                                          m_shader_buffers{};
        mge::com_ptr<ID3D12RootSignature> m_root_signature;
    };

    inline program& dx12_program(mge::program& p)
    {
        return static_cast<program&>(p);
    }

    inline const program& dx12_program(const mge::program& p)
    {
        return static_cast<const program&>(p);
    }

} // namespace mge::dx12