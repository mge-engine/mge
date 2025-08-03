// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "dx11.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"
#include "window.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(DX11);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, directx11, debug, "Enable DirectX 11 debug mode", false);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(bool,
                                      directx11,
                                      software_device,
                                      "Use the DirextX 11 software device",
                                      false);
} // namespace mge

namespace mge::dx11 {
    render_system::render_system()
    {
        MGE_DEBUG_TRACE(DX11, "Creating DirectX 11 render system");
        init_capabilities();
    }

    void render_system::init_capabilities()
    {
        class capabilities : public mge::render_system::capabilities
        {
        public:
            capabilities()
            {
                shader_language hlsl{"hlsl"sv, mge::semantic_version(5, 0)};
                m_shader_languages.push_back(hlsl);
                shader_format bytecode{"bytecode"sv,
                                       mge::semantic_version(5, 0)};
                m_shader_formats.push_back(bytecode);
            }
            ~capabilities() = default;

            std::span<const mge::shader_language>
            shader_languages() const override
            {
                return std::span<const mge::shader_language>(
                    m_shader_languages.data(),
                    m_shader_languages.size());
            }

            std::span<const mge::shader_format> shader_formats() const override
            {
                return std::span<const mge::shader_format>(
                    m_shader_formats.data(),
                    m_shader_formats.size());
            }

        private:
            std::vector<mge::shader_language> m_shader_languages;
            std::vector<mge::shader_format>   m_shader_formats;
        };

        m_capabilities = std::make_unique<capabilities>();
    }

    std::span<mge::monitor_ref> render_system::monitors()
    {
        return mge::win32::monitor::all_monitors();
    }

    mge::window_ref
    render_system::create_window(const mge::extent&         extent,
                                 const mge::window_options& options)
    {
        mge::window_ref ref = std::make_shared<window>(*this, extent, options);
        return ref;
    }

    bool render_system::debug() const
    {
        return MGE_PARAMETER(directx11, debug).get();
    }

    bool render_system::software_device() const
    {
        return MGE_PARAMETER(directx11, software_device).get();
    }

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                directx11,
                                dx11);
} // namespace mge::dx11
