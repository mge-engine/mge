// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "dx12.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "mge/win32/com_ptr.hpp"
#include "window.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(DX12);

    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, directx12, debug, "Enable DirectX 12 debug mode", false);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, directx12, warp, "Enable DirectX 12 warp device", false);

} // namespace mge

namespace mge::dx12 {
    render_system::render_system()
        : m_tearing_support(false)
    {
        MGE_DEBUG_TRACE_STREAM(DX12) << "Creating DirectX 12 render system";
        enable_debug_layer();
        check_tearing_support();
        init_capabilities();
    }

    void render_system::init_capabilities()
    {
        class capabilities : public mge::render_system::capabilities
        {
        public:
            capabilities()
            {
                // TODO: resolve actual shader model (up to version 6.6)
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
        return MGE_PARAMETER(directx12, debug).get();
    }

    bool render_system::warp() const
    {
        return MGE_PARAMETER(directx12, warp).get();
    }

    bool render_system::tearing_support() const
    {
        return m_tearing_support;
    }

    void render_system::enable_debug_layer()
    {
        if (debug()) {
            MGE_DEBUG_TRACE_STREAM(DX12) << "Enable debug layer";
            mge::com_ptr<ID3D12Debug1> debug_interface;
            auto rc = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface));
            CHECK_HRESULT(rc, , D3D12GetDebugInterface);
            debug_interface->EnableDebugLayer();
            debug_interface->SetEnableGPUBasedValidation(TRUE);
        }
    }

    void render_system::check_tearing_support()
    {
        mge::com_ptr<IDXGIFactory4> factory4;
        if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4)))) {
            mge::com_ptr<IDXGIFactory5> factory5;
            if (SUCCEEDED(factory4.As(&factory5))) {
                BOOL allow_tearing = FALSE;
                if (FAILED(factory5->CheckFeatureSupport(
                        DXGI_FEATURE_PRESENT_ALLOW_TEARING,
                        &allow_tearing,
                        sizeof(allow_tearing)))) {
                    allow_tearing = FALSE;
                }
                if (allow_tearing) {
                    m_tearing_support = true;
                }
            }
        }
    }

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                directx12,
                                dx12);
} // namespace mge::dx12
