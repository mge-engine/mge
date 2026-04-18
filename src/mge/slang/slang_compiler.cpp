// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "slang_compiler.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"

#include <slang-com-ptr.h>
#include <slang.h>

namespace mge {
    MGE_DEFINE_TRACE(SLANG);
}

namespace mge {

    static mge::shader_type shader_type_from_slang_stage(SlangStage stage)
    {
        switch (stage) {
        case SLANG_STAGE_VERTEX:
            return mge::shader_type::VERTEX;
        case SLANG_STAGE_FRAGMENT:
            return mge::shader_type::FRAGMENT;
        case SLANG_STAGE_COMPUTE:
            return mge::shader_type::COMPUTE;
        case SLANG_STAGE_HULL:
            return mge::shader_type::TESSELATION_CONTROL;
        case SLANG_STAGE_DOMAIN:
            return mge::shader_type::TESSELATION_EVALUATION;
        case SLANG_STAGE_GEOMETRY:
            return mge::shader_type::GEOMETRY;
        default:
            MGE_THROW(mge::runtime_exception)
                << "Unsupported Slang shader stage: " << (int)stage;
        }
    }

    static std::string diagnostics_string(slang::IBlob* diagnostics)
    {
        if (diagnostics) {
            return std::string(
                static_cast<const char*>(diagnostics->getBufferPointer()),
                diagnostics->getBufferSize());
        }
        return {};
    }

    slang_compile_result slang_compile(slang_target     target,
                                       std::string_view source)
    {
        SlangCompileTarget slang_format;
        const char*        profile_name;

        switch (target) {
        case slang_target::DXBC:
            slang_format = SLANG_DXBC;
            profile_name = "sm_5_0";
            break;
        case slang_target::SPIRV:
            slang_format = SLANG_SPIRV;
            profile_name = "glsl_460";
            break;
        case slang_target::GLSL:
            slang_format = SLANG_GLSL;
            profile_name = "glsl_410";
            break;
        default:
            MGE_THROW(mge::runtime_exception)
                << "Unsupported Slang target: " << (int)target;
        }

        Slang::ComPtr<slang::IGlobalSession> global_session;
        auto rc = slang::createGlobalSession(global_session.writeRef());
        if (SLANG_FAILED(rc)) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to create Slang global session";
        }

        slang::TargetDesc target_desc = {};
        target_desc.format = slang_format;
        target_desc.profile = global_session->findProfile(profile_name);

        slang::CompilerOptionEntry no_mangle_option = {};
        no_mangle_option.name = slang::CompilerOptionName::NoMangle;
        no_mangle_option.value.kind = slang::CompilerOptionValueKind::Int;
        no_mangle_option.value.intValue0 = 1;

        slang::SessionDesc session_desc = {};
        session_desc.targets = &target_desc;
        session_desc.targetCount = 1;
        session_desc.compilerOptionEntries = &no_mangle_option;
        session_desc.compilerOptionEntryCount = 1;

        Slang::ComPtr<slang::ISession> session;
        rc = global_session->createSession(session_desc, session.writeRef());
        if (SLANG_FAILED(rc)) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to create Slang session";
        }

        Slang::ComPtr<slang::IBlob> diagnostics;
        slang::IModule*             module =
            session->loadModuleFromSourceString("shader",
                                                "shader.slang",
                                                std::string(source).c_str(),
                                                diagnostics.writeRef());
        if (!module) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to load Slang module: "
                << diagnostics_string(diagnostics.get());
        }

        SlangInt32 entry_point_count = module->getDefinedEntryPointCount();
        if (entry_point_count == 0) {
            MGE_THROW(mge::runtime_exception)
                << "Slang module has no entry points";
        }

        MGE_DEBUG_TRACE(SLANG,
                        "Slang module has {} entry points",
                        entry_point_count);

        std::vector<Slang::ComPtr<slang::IEntryPoint>> entry_points;
        std::vector<slang::IComponentType*>            components;
        components.push_back(module);

        for (SlangInt32 i = 0; i < entry_point_count; ++i) {
            Slang::ComPtr<slang::IEntryPoint> entry_point;
            rc = module->getDefinedEntryPoint(i, entry_point.writeRef());
            if (SLANG_FAILED(rc)) {
                MGE_THROW(mge::runtime_exception)
                    << "Failed to get entry point " << i;
            }
            components.push_back(entry_point.get());
            entry_points.push_back(entry_point);
        }

        Slang::ComPtr<slang::IComponentType> composed_program;
        rc = session->createCompositeComponentType(
            components.data(),
            static_cast<SlangInt>(components.size()),
            composed_program.writeRef(),
            diagnostics.writeRef());
        if (SLANG_FAILED(rc)) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to compose Slang program: "
                << diagnostics_string(diagnostics.get());
        }

        Slang::ComPtr<slang::IComponentType> linked_program;
        rc = composed_program->link(linked_program.writeRef(),
                                    diagnostics.writeRef());
        if (SLANG_FAILED(rc)) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to link Slang program: "
                << diagnostics_string(diagnostics.get());
        }

        slang::ProgramLayout* layout =
            linked_program->getLayout(0, diagnostics.writeRef());
        if (!layout) {
            MGE_THROW(mge::runtime_exception)
                << "Failed to get Slang program layout";
        }

        slang_compile_result result;

        for (SlangInt32 i = 0; i < entry_point_count; ++i) {
            Slang::ComPtr<slang::IBlob> code;
            rc = linked_program->getEntryPointCode(i,
                                                   0,
                                                   code.writeRef(),
                                                   diagnostics.writeRef());
            if (SLANG_FAILED(rc)) {
                MGE_THROW(mge::runtime_exception)
                    << "Failed to get entry point code for index " << i << ": "
                    << diagnostics_string(diagnostics.get());
            }

            auto*            reflection = layout->getEntryPointByIndex(i);
            SlangStage       stage = reflection->getStage();
            mge::shader_type type = shader_type_from_slang_stage(stage);
            const char*      name = reflection->getName();

            MGE_DEBUG_TRACE(SLANG,
                            "Slang entry point {}: name='{}', stage={}, "
                            "code size={} bytes",
                            i,
                            name ? name : "(null)",
                            type,
                            code->getBufferSize());

            slang_shader_code shader_code;
            shader_code.type = type;
            shader_code.entry_point_name = "main";

            if (target == slang_target::GLSL) {
                shader_code.text_code.assign(
                    static_cast<const char*>(code->getBufferPointer()),
                    code->getBufferSize());
            } else {
                shader_code.binary_code.resize(code->getBufferSize());
                memcpy(shader_code.binary_code.data(),
                       code->getBufferPointer(),
                       code->getBufferSize());
            }

            result.shader_code[type] = std::move(shader_code);
        }

        return result;
    }

} // namespace mge
