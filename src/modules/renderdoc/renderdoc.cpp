// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/shared_library.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_debugger.hpp"

#include "renderdoc_app.h"

namespace mge {
    MGE_DEFINE_TRACE(RENDERDOC);
}

namespace mge::renderdoc {
    class renderdoc_frame_debugger : public mge::frame_debugger
    {
    public:
        renderdoc_frame_debugger();
        virtual ~renderdoc_frame_debugger();

        virtual std::string_view      name() const noexcept override;
        virtual mge::semantic_version version() const noexcept override;

        virtual void configure() override;
        virtual void
        start_capture(void* native_window_handle = nullptr) override;
        virtual void end_capture(void* native_window_handle = nullptr) override;
        virtual void capture() override;
        virtual bool capturing() const override;

    private:
        bool renderdoc_library_loaded();
        bool try_load_renderdoc_library(const std::filesystem::path& path);

        bool                                 m_enabled{false};
        std::unique_ptr<mge::shared_library> m_library;
        RENDERDOC_API_1_6_0*                 m_renderdoc_api{nullptr};
    };
#ifdef MGE_OS_WINDOWS
    const std::filesystem::path renderdoc_lib_name = "renderdoc.dll";
#elif defined(MGE_OS_LINUX)
    const std::filesystem::path renderdoc_lib_name = "librenderdoc.so";
#else
#    error RenderDoc not supported on this platform
#endif

    renderdoc_frame_debugger::renderdoc_frame_debugger() {}

    renderdoc_frame_debugger::~renderdoc_frame_debugger() {}

    std::string_view renderdoc_frame_debugger::name() const noexcept
    {
        return "RenderDoc";
    }

    mge::semantic_version renderdoc_frame_debugger::version() const noexcept
    {
        if (m_renderdoc_api) {
            int major = 0;
            int minor = 0;
            int patch = 0;
            m_renderdoc_api->GetAPIVersion(&major, &minor, &patch);
            return mge::semantic_version(major, minor, patch);
        } else {
            return mge::semantic_version(0, 0, 0);
        }
    }

    bool renderdoc_frame_debugger::renderdoc_library_loaded()
    {
        return shared_library::loaded(renderdoc_lib_name);
    }

    bool renderdoc_frame_debugger::try_load_renderdoc_library(
        const std::filesystem::path& path)
    {
        try {
            m_library = std::make_unique<shared_library>(
                path,
                shared_library::use_search_path{});
            MGE_DEBUG_TRACE(RENDERDOC,
                            "Successfully loaded RenderDoc library from {}",
                            path.string());
            return true;
        } catch (const mge::exception& e) {
            MGE_DEBUG_TRACE(RENDERDOC,
                            "Failed to load RenderDoc library from {}: {}",
                            path.string(),
                            e.what());
        }
        return false;
    }

    void renderdoc_frame_debugger::configure()
    {
        if (renderdoc_library_loaded()) {
            m_enabled = false;
            MGE_DEBUG_TRACE(
                RENDERDOC,
                "RenderDoc library was loaded already, skipping setup");
            return;
        }
        MGE_DEBUG_TRACE(RENDERDOC, "Configuring RenderDoc frame debugger");

        if (!try_load_renderdoc_library(renderdoc_lib_name)) {

            const char* env_path = std::getenv("MGE_RENDERDOC_LIBRARY_PATH");
            if (env_path) {
                MGE_DEBUG_TRACE(RENDERDOC,
                                "Trying to load RenderDoc library from "
                                "MGE_RENDERDOC_LIBRARY_PATH: {}",
                                env_path);
                std::filesystem::path env_path_fs(env_path);

                if (env_path_fs.filename() != renderdoc_lib_name) {
                    env_path_fs /= renderdoc_lib_name;
                }
                if (!try_load_renderdoc_library(env_path_fs)) {
                    MGE_DEBUG_TRACE(RENDERDOC,
                                    "Could not load RenderDoc library from "
                                    "MGE_RENDERDOC_LIBRARY_PATH, disabling "
                                    "frame debugger");
                    m_enabled = false;
                    return;
                }
            }
        }
        pRENDERDOC_GetAPI get_api = nullptr;
        get_api = reinterpret_cast<pRENDERDOC_GetAPI>(
            m_library->symbol("RENDERDOC_GetAPI"));
        if (!get_api) {
            MGE_DEBUG_TRACE(RENDERDOC,
                            "Could not find RENDERDOC_GetAPI symbol, disabling "
                            "frame debugger");
            m_enabled = false;
            return;
        }

        int ret = get_api(eRENDERDOC_API_Version_1_6_0,
                          reinterpret_cast<void**>(&m_renderdoc_api));
        if (ret != 1) {
            MGE_DEBUG_TRACE(RENDERDOC,
                            "Could not get RenderDoc API, disabling frame "
                            "debugger");
            m_enabled = false;
            return;
        }

        auto executable_name = mge::executable_name();
        m_renderdoc_api->SetCaptureFilePathTemplate((executable_name).c_str());

        auto lfp = m_renderdoc_api->GetLogFilePathTemplate();
        MGE_DEBUG_TRACE(RENDERDOC,
                        "RenderDoc frame debugger configured, log file path "
                        "template: {}",
                        lfp);

        m_enabled = true;
    }

    void renderdoc_frame_debugger::start_capture(void* native_window_handle)
    {
        if (m_renderdoc_api) {
            MGE_DEBUG_TRACE(RENDERDOC, "Starting frame capture");
            m_renderdoc_api->StartFrameCapture(nullptr, native_window_handle);
        }
    }

    void renderdoc_frame_debugger::end_capture(void* native_window_handle)
    {
        if (m_renderdoc_api) {
            MGE_DEBUG_TRACE(RENDERDOC, "Ending frame capture");
            m_renderdoc_api->EndFrameCapture(nullptr, native_window_handle);
        }
    }

    void renderdoc_frame_debugger::capture()
    {
        if (m_renderdoc_api) {
            m_renderdoc_api->TriggerCapture();
        }
    }

    bool renderdoc_frame_debugger::capturing() const
    {
        if (m_renderdoc_api) {
            return m_renderdoc_api->IsFrameCapturing() != 0;
        }
        return false;
    }

    MGE_REGISTER_IMPLEMENTATION(renderdoc_frame_debugger,
                                mge::frame_debugger,
                                renderdoc);

} // namespace mge::renderdoc
