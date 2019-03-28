#include "render_context.hpp"
#include "window.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/to_utf8.hpp"

MGE_USE_LOG(DX12);

namespace dx12 {
    render_context::render_context(window *w, const system_config& config)
        : mge::render_context(w)
        ,m_window(w)
        ,m_feature_level((D3D_FEATURE_LEVEL) 0)
        ,m_frame_index(0)
        ,m_rtv_descriptor_size(0)
    {
        init_context(config);
    }

    render_context::~render_context()
    {}

    void
    render_context::flush()
    {
        return;
    }

    void
    render_context::create_factory()
    {
        MGE_DEBUG_LOG(DX12) << "Create DXGI factory";
        IDXGIFactory4 *factory = nullptr;
        HRESULT rc = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void **) &factory);
        CHECK_HRESULT(rc, , CreateDXGIFactory1);
        if(!factory) {
            MGE_THROW(mge::null_pointer);
        }
        m_dxgi_factory.reset(factory);
    }

    void
    render_context::select_adapter()
    {
        MGE_DEBUG_LOG(DX12) << "Selecting adapter";
        IDXGIAdapter3 *adapter = nullptr;
        uint32_t index = 0;
        while(m_dxgi_factory->EnumAdapters(index, (IDXGIAdapter **)&adapter)) {
            MGE_DEBUG_LOG(DX12) << "Adapter " << index;
            DXGI_ADAPTER_DESC1 desc;
            HRESULT rc = adapter->GetDesc1(&desc);
            if(SUCCEEDED(rc)) {
                MGE_DEBUG_LOG(DX12) << "Description: " << mge::to_utf8(&desc.Description[0]);
                MGE_DEBUG_LOG(DX12) << "Vendor: " << std::hex << desc.VendorId << std::dec;
            }
            m_adapter.reset(adapter);
            if((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0) {
                if(setup_device()) {
                    break;
                }
            } else {
                MGE_DEBUG_LOG(DX12) << "Skip software driver";
                m_adapter.reset();
            }
            ++index;
        }
    }

    bool
    render_context::setup_device()
    {
        D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_12_1,
                                              D3D_FEATURE_LEVEL_12_0,
                                              D3D_FEATURE_LEVEL_11_1,
                                              D3D_FEATURE_LEVEL_11_0, };
        ID3D12Device *device;
        HRESULT hr = 0;
        for(const auto level : featurelevels) {
            hr = D3D12CreateDevice(m_adapter.get(),
                                   level,
                                   IID_ID3D12Device,
                                   (void **)&device);
            if (SUCCEEDED(hr)) {
                m_feature_level = level;
                m_device.reset(device);
                return true;
            }
        }

        return false;
    }

    void
    render_context::init_context(const system_config& config)
    {
        create_factory();
        select_adapter();
    }
}