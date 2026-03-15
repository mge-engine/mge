// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/graphics/data_type.hpp"
#include "mge/graphics/point.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/vertex_format.hpp"

namespace mge::reflection {

    class graphics_reflector : public reflector
    {
    public:
        graphics_reflector() = default;
        ~graphics_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(type<mge::data_type>());
            mge(type<mge::topology>());
            mge(type<mge::point>()
                    .constructor<uint32_t, uint32_t>()
                    .field("x", &mge::point::x)
                    .field("y", &mge::point::y));
            mge(type<mge::vertex_format>()
                    .constructor<mge::data_type, size_t>()
                    .method("binary_size", &mge::vertex_format::binary_size));
            mge(type<mge::render_system>()
                    .static_method("create",
                                   static_cast<mge::render_system_ref (*)()>(
                                       &mge::render_system::create))
                    .static_method(
                        "create",
                        static_cast<mge::render_system_ref (*)(
                            std::string_view)>(&mge::render_system::create))
                    .method(
                        "create_window",
                        static_cast<mge::window_ref (mge::render_system::*)()>(
                            &mge::render_system::create_window))
                    .method("system_capabilities",
                            &mge::render_system::system_capabilities)
                    .method("monitors", &mge::render_system::monitors)
                    .method("frame_debugger",
                            &mge::render_system::frame_debugger));
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 3> deps{
                {"core", "std", "math"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(graphics_reflector,
                                mge::reflection::reflector,
                                graphics);
} // namespace mge::reflection
