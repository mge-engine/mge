// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_debugger.hpp"

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
        virtual void start_capture() override;
        virtual void end_capture() override;
        virtual void capture() override;
    };

    renderdoc_frame_debugger::renderdoc_frame_debugger() {}

    renderdoc_frame_debugger::~renderdoc_frame_debugger() {}

    std::string_view renderdoc_frame_debugger::name() const noexcept
    {
        return "RenderDoc";
    }

    mge::semantic_version renderdoc_frame_debugger::version() const noexcept
    {
        return mge::semantic_version(1, 0, 0);
    }

    void renderdoc_frame_debugger::configure()
    {
        // Configuration code for RenderDoc integration would go here
    }

    void renderdoc_frame_debugger::start_capture()
    {
        // Code to start frame capture with RenderDoc would go here
    }

    void renderdoc_frame_debugger::end_capture()
    {
        // Code to end frame capture with RenderDoc would go here
    }

    void renderdoc_frame_debugger::capture()
    {
        // Code to capture a frame with RenderDoc would go here
    }

    MGE_REGISTER_IMPLEMENTATION(renderdoc_frame_debugger,
                                mge::frame_debugger,
                                renderdoc);

} // namespace mge::renderdoc
