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

    static mge::uniform_data_type
    uniform_type_from_slang(slang::TypeReflection* type)
    {
        auto kind = type->getKind();

        if (kind == slang::TypeReflection::Kind::Scalar) {
            switch (type->getScalarType()) {
            case slang::TypeReflection::ScalarType::Float32:
                return mge::uniform_data_type::FLOAT;
            case slang::TypeReflection::ScalarType::Float64:
                return mge::uniform_data_type::DOUBLE;
            case slang::TypeReflection::ScalarType::Int32:
                return mge::uniform_data_type::INT32;
            case slang::TypeReflection::ScalarType::UInt32:
                return mge::uniform_data_type::UINT32;
            case slang::TypeReflection::ScalarType::Int16:
                return mge::uniform_data_type::INT16;
            case slang::TypeReflection::ScalarType::UInt16:
                return mge::uniform_data_type::UINT16;
            case slang::TypeReflection::ScalarType::Int8:
                return mge::uniform_data_type::INT8;
            case slang::TypeReflection::ScalarType::UInt8:
                return mge::uniform_data_type::UINT8;
            case slang::TypeReflection::ScalarType::Float16:
                return mge::uniform_data_type::HALF;
            case slang::TypeReflection::ScalarType::Int64:
                return mge::uniform_data_type::INT64;
            case slang::TypeReflection::ScalarType::UInt64:
                return mge::uniform_data_type::UINT64;
            default:
                return mge::uniform_data_type::UNKNOWN;
            }
        }

        if (kind == slang::TypeReflection::Kind::Vector) {
            auto scalar = type->getScalarType();
            auto cols = type->getColumnCount();

            if (scalar == slang::TypeReflection::ScalarType::Float32) {
                switch (cols) {
                case 2:
                    return mge::uniform_data_type::FLOAT_VEC2;
                case 3:
                    return mge::uniform_data_type::FLOAT_VEC3;
                case 4:
                    return mge::uniform_data_type::FLOAT_VEC4;
                default:
                    return mge::uniform_data_type::UNKNOWN;
                }
            }
            if (scalar == slang::TypeReflection::ScalarType::Int32) {
                switch (cols) {
                case 2:
                    return mge::uniform_data_type::INT_VEC2;
                case 3:
                    return mge::uniform_data_type::INT_VEC3;
                case 4:
                    return mge::uniform_data_type::INT_VEC4;
                default:
                    return mge::uniform_data_type::UNKNOWN;
                }
            }
            if (scalar == slang::TypeReflection::ScalarType::UInt32) {
                switch (cols) {
                case 2:
                    return mge::uniform_data_type::UINT_VEC2;
                case 3:
                    return mge::uniform_data_type::UINT_VEC3;
                case 4:
                    return mge::uniform_data_type::UINT_VEC4;
                default:
                    return mge::uniform_data_type::UNKNOWN;
                }
            }
            if (scalar == slang::TypeReflection::ScalarType::Float64) {
                switch (cols) {
                case 2:
                    return mge::uniform_data_type::DOUBLE_VEC2;
                case 3:
                    return mge::uniform_data_type::DOUBLE_VEC3;
                case 4:
                    return mge::uniform_data_type::DOUBLE_VEC4;
                default:
                    return mge::uniform_data_type::UNKNOWN;
                }
            }
            return mge::uniform_data_type::UNKNOWN;
        }

        if (kind == slang::TypeReflection::Kind::Matrix) {
            auto scalar = type->getScalarType();
            auto rows = type->getRowCount();
            auto cols = type->getColumnCount();

            if (scalar == slang::TypeReflection::ScalarType::Float32) {
                if (rows == cols) {
                    switch (rows) {
                    case 2:
                        return mge::uniform_data_type::FLOAT_MAT2;
                    case 3:
                        return mge::uniform_data_type::FLOAT_MAT3;
                    case 4:
                        return mge::uniform_data_type::FLOAT_MAT4;
                    default:
                        return mge::uniform_data_type::UNKNOWN;
                    }
                }
                if (rows == 2 && cols == 3)
                    return mge::uniform_data_type::FLOAT_MAT2x3;
                if (rows == 2 && cols == 4)
                    return mge::uniform_data_type::FLOAT_MAT2x4;
                if (rows == 3 && cols == 2)
                    return mge::uniform_data_type::FLOAT_MAT3x2;
                if (rows == 3 && cols == 4)
                    return mge::uniform_data_type::FLOAT_MAT3x4;
                if (rows == 4 && cols == 2)
                    return mge::uniform_data_type::FLOAT_MAT4x2;
                if (rows == 4 && cols == 3)
                    return mge::uniform_data_type::FLOAT_MAT4x3;
            }

            if (scalar == slang::TypeReflection::ScalarType::Float64) {
                if (rows == cols) {
                    switch (rows) {
                    case 2:
                        return mge::uniform_data_type::DOUBLE_MAT2;
                    case 3:
                        return mge::uniform_data_type::DOUBLE_MAT3;
                    case 4:
                        return mge::uniform_data_type::DOUBLE_MAT4;
                    default:
                        return mge::uniform_data_type::UNKNOWN;
                    }
                }
                if (rows == 2 && cols == 3)
                    return mge::uniform_data_type::DOUBLE_MAT2x3;
                if (rows == 2 && cols == 4)
                    return mge::uniform_data_type::DOUBLE_MAT2x4;
                if (rows == 3 && cols == 2)
                    return mge::uniform_data_type::DOUBLE_MAT3x2;
                if (rows == 3 && cols == 4)
                    return mge::uniform_data_type::DOUBLE_MAT3x4;
                if (rows == 4 && cols == 2)
                    return mge::uniform_data_type::DOUBLE_MAT4x2;
                if (rows == 4 && cols == 3)
                    return mge::uniform_data_type::DOUBLE_MAT4x3;
            }

            return mge::uniform_data_type::UNKNOWN;
        }

        return mge::uniform_data_type::UNKNOWN;
    }

    static mge::data_type attribute_type_from_slang(slang::TypeReflection* type)
    {
        auto kind = type->getKind();

        if (kind == slang::TypeReflection::Kind::Scalar) {
            switch (type->getScalarType()) {
            case slang::TypeReflection::ScalarType::Float32:
                return mge::data_type::FLOAT;
            case slang::TypeReflection::ScalarType::Float64:
                return mge::data_type::DOUBLE;
            case slang::TypeReflection::ScalarType::Int32:
                return mge::data_type::INT32;
            case slang::TypeReflection::ScalarType::UInt32:
                return mge::data_type::UINT32;
            default:
                return mge::data_type::UNKNOWN;
            }
        }

        if (kind == slang::TypeReflection::Kind::Vector) {
            auto scalar = type->getScalarType();
            auto cols = type->getColumnCount();

            if (scalar == slang::TypeReflection::ScalarType::Float32) {
                switch (cols) {
                case 2:
                    return mge::data_type::FLOAT_VEC2;
                case 3:
                    return mge::data_type::FLOAT_VEC3;
                case 4:
                    return mge::data_type::FLOAT_VEC4;
                default:
                    return mge::data_type::UNKNOWN;
                }
            }
            if (scalar == slang::TypeReflection::ScalarType::Int32) {
                switch (cols) {
                case 2:
                    return mge::data_type::INT_VEC2;
                case 3:
                    return mge::data_type::INT_VEC3;
                case 4:
                    return mge::data_type::INT_VEC4;
                default:
                    return mge::data_type::UNKNOWN;
                }
            }
            if (scalar == slang::TypeReflection::ScalarType::UInt32) {
                switch (cols) {
                case 2:
                    return mge::data_type::UINT_VEC2;
                case 3:
                    return mge::data_type::UINT_VEC3;
                case 4:
                    return mge::data_type::UINT_VEC4;
                default:
                    return mge::data_type::UNKNOWN;
                }
            }
            if (scalar == slang::TypeReflection::ScalarType::Float64) {
                switch (cols) {
                case 2:
                    return mge::data_type::DOUBLE_VEC2;
                case 3:
                    return mge::data_type::DOUBLE_VEC3;
                case 4:
                    return mge::data_type::DOUBLE_VEC4;
                default:
                    return mge::data_type::UNKNOWN;
                }
            }
            return mge::data_type::UNKNOWN;
        }

        if (kind == slang::TypeReflection::Kind::Matrix) {
            auto scalar = type->getScalarType();
            auto rows = type->getRowCount();
            auto cols = type->getColumnCount();

            if (scalar == slang::TypeReflection::ScalarType::Float32) {
                if (rows == cols) {
                    switch (rows) {
                    case 2:
                        return mge::data_type::FLOAT_MAT2;
                    case 3:
                        return mge::data_type::FLOAT_MAT3;
                    case 4:
                        return mge::data_type::FLOAT_MAT4;
                    default:
                        return mge::data_type::UNKNOWN;
                    }
                }
                if (rows == 2 && cols == 3)
                    return mge::data_type::FLOAT_MAT2x3;
                if (rows == 2 && cols == 4)
                    return mge::data_type::FLOAT_MAT2x4;
                if (rows == 3 && cols == 2)
                    return mge::data_type::FLOAT_MAT3x2;
                if (rows == 3 && cols == 4)
                    return mge::data_type::FLOAT_MAT3x4;
                if (rows == 4 && cols == 2)
                    return mge::data_type::FLOAT_MAT4x2;
                if (rows == 4 && cols == 3)
                    return mge::data_type::FLOAT_MAT4x3;
            }

            return mge::data_type::UNKNOWN;
        }

        return mge::data_type::UNKNOWN;
    }

    static void
    collect_uniform_block_members(slang::TypeLayoutReflection* type_layout,
                                  program::uniform_list&       uniforms)
    {
        unsigned field_count = type_layout->getFieldCount();
        for (unsigned f = 0; f < field_count; ++f) {
            auto* field = type_layout->getFieldByIndex(f);
            auto* var = field->getVariable();
            auto* field_type = var->getType();

            const char* field_name = var->getName();
            auto        udt = uniform_type_from_slang(field_type);

            uint32_t array_size = 1;
            if (field_type->getKind() == slang::TypeReflection::Kind::Array) {
                array_size =
                    static_cast<uint32_t>(field_type->getElementCount());
                field_type = field_type->getElementType();
                udt = uniform_type_from_slang(field_type);
            }

            uint32_t offset = static_cast<uint32_t>(
                field->getOffset(SLANG_PARAMETER_CATEGORY_UNIFORM));

            MGE_DEBUG_TRACE(SLANG,
                            "  Field '{}': type={}, array_size={}, "
                            "offset={}",
                            field_name ? field_name : "(null)",
                            udt,
                            array_size,
                            offset);

            uniforms.push_back(
                {field_name ? field_name : "", udt, array_size, offset});
        }
    }

    static void extract_reflection(slang::ProgramLayout* layout,
                                   slang_compile_result& result)
    {
        // Extract global parameters (uniform buffers, samplers, loose
        // uniforms)
        unsigned param_count = layout->getParameterCount();
        MGE_DEBUG_TRACE(SLANG,
                        "Slang reflection: {} global parameters",
                        param_count);

        for (unsigned i = 0; i < param_count; ++i) {
            auto*       param = layout->getParameterByIndex(i);
            auto        category = param->getCategory();
            auto*       var = param->getVariable();
            const char* param_name = var ? var->getName() : nullptr;
            auto*       type_layout = param->getTypeLayout();

            MGE_DEBUG_TRACE(SLANG,
                            "  Parameter {}: name='{}', category={}",
                            i,
                            param_name ? param_name : "(null)",
                            static_cast<int>(category));

            if (category == slang::ParameterCategory::ConstantBuffer) {
                // Uniform buffer block
                auto* element_type_layout = type_layout->getElementTypeLayout();

                unsigned binding = param->getBindingIndex();

                program::uniform_block_metadata block;
                block.name = param_name ? param_name : "";
                block.location = binding;

                MGE_DEBUG_TRACE(SLANG,
                                "  Constant buffer '{}' at binding {}",
                                block.name,
                                binding);

                collect_uniform_block_members(element_type_layout,
                                              block.uniforms);

                result.uniform_buffers.push_back(std::move(block));
            } else if (category == slang::ParameterCategory::SamplerState) {
                unsigned binding = param->getBindingIndex();
                result.sampler_bindings.push_back(
                    {param_name ? param_name : "", binding});
                MGE_DEBUG_TRACE(SLANG,
                                "  Sampler '{}' at binding {}",
                                param_name ? param_name : "(null)",
                                binding);
            } else if (category == slang::ParameterCategory::ShaderResource) {
                // Combined image sampler or texture
                unsigned binding = param->getBindingIndex();
                result.sampler_bindings.push_back(
                    {param_name ? param_name : "", binding});
                MGE_DEBUG_TRACE(SLANG,
                                "  Shader resource '{}' at binding {}",
                                param_name ? param_name : "(null)",
                                binding);
            } else if (category ==
                       slang::ParameterCategory::DescriptorTableSlot) {
                // Descriptor table slot - can be textures, samplers,
                // or constant buffers depending on target
                auto kind = type_layout->getKind();
                if (kind == slang::TypeReflection::Kind::ConstantBuffer) {
                    // Constant buffer in a descriptor table slot
                    auto* element_type_layout =
                        type_layout->getElementTypeLayout();
                    unsigned binding = param->getBindingIndex();

                    program::uniform_block_metadata block;
                    block.name = param_name ? param_name : "";
                    block.location = binding;

                    MGE_DEBUG_TRACE(SLANG,
                                    "  Descriptor table constant buffer "
                                    "'{}' at binding {}",
                                    block.name,
                                    binding);

                    collect_uniform_block_members(element_type_layout,
                                                  block.uniforms);

                    result.uniform_buffers.push_back(std::move(block));
                } else {
                    SlangInt range_count = type_layout->getBindingRangeCount();
                    auto     binding_type =
                        range_count > 0 ? type_layout->getBindingRangeType(0)
                                        : slang::BindingType::Unknown;
                    if (binding_type ==
                            slang::BindingType::CombinedTextureSampler ||
                        binding_type == slang::BindingType::Texture ||
                        binding_type == slang::BindingType::Sampler) {
                        unsigned binding = param->getBindingIndex();
                        result.sampler_bindings.push_back(
                            {param_name ? param_name : "", binding});
                        MGE_DEBUG_TRACE(SLANG,
                                        "  Descriptor table slot '{}' at "
                                        "binding {}",
                                        param_name ? param_name : "(null)",
                                        binding);
                    } else {
                        MGE_DEBUG_TRACE(SLANG,
                                        "  Unhandled descriptor table slot "
                                        "'{}' binding_type={}",
                                        param_name ? param_name : "(null)",
                                        static_cast<int>(binding_type));
                    }
                }
            } else if (category == slang::ParameterCategory::Uniform) {
                // Loose uniform (not in a block)
                auto* var_type = var ? var->getType() : nullptr;
                if (var_type) {
                    auto udt = uniform_type_from_slang(var_type);

                    uint32_t array_size = 1;
                    if (var_type->getKind() ==
                        slang::TypeReflection::Kind::Array) {
                        array_size =
                            static_cast<uint32_t>(var_type->getElementCount());
                    }
                    uint32_t offset = static_cast<uint32_t>(
                        param->getOffset(SLANG_PARAMETER_CATEGORY_UNIFORM));

                    result.uniforms.push_back({param_name ? param_name : "",
                                               udt,
                                               array_size,
                                               offset});
                }
            } else if (category == slang::ParameterCategory::Mixed) {
                // Mixed category - may contain a constant buffer with
                // additional resources. Check binding ranges.
                auto kind = type_layout->getKind();
                if (kind == slang::TypeReflection::Kind::ConstantBuffer) {
                    auto* element_type_layout =
                        type_layout->getElementTypeLayout();
                    unsigned binding = param->getBindingIndex();

                    program::uniform_block_metadata block;
                    block.name = param_name ? param_name : "";
                    block.location = binding;

                    MGE_DEBUG_TRACE(
                        SLANG,
                        "  Mixed constant buffer '{}' at binding {}",
                        block.name,
                        binding);

                    collect_uniform_block_members(element_type_layout,
                                                  block.uniforms);

                    result.uniform_buffers.push_back(std::move(block));

                    // Check for embedded samplers/textures in the
                    // element type
                    SlangInt range_count =
                        element_type_layout->getBindingRangeCount();
                    for (SlangInt r = 0; r < range_count; ++r) {
                        auto bt = element_type_layout->getBindingRangeType(r);
                        if (bt == slang::BindingType::CombinedTextureSampler ||
                            bt == slang::BindingType::Texture ||
                            bt == slang::BindingType::Sampler) {
                            auto* leaf_var =
                                element_type_layout
                                    ->getBindingRangeLeafVariable(r);
                            const char* res_name =
                                leaf_var ? leaf_var->getName() : nullptr;
                            // Binding index relative to the
                            // descriptor set
                            unsigned res_binding = static_cast<unsigned>(
                                element_type_layout
                                    ->getDescriptorSetDescriptorRangeIndexOffset(
                                        0,
                                        r));
                            result.sampler_bindings.push_back(
                                {res_name ? res_name : "", res_binding});
                            MGE_DEBUG_TRACE(SLANG,
                                            "  Embedded resource '{}' at "
                                            "binding {}",
                                            res_name ? res_name : "(null)",
                                            res_binding);
                        }
                    }
                }
            }
        }

        // Extract vertex attributes from vertex entry point
        SlangUInt ep_count = layout->getEntryPointCount();
        for (SlangUInt ep = 0; ep < ep_count; ++ep) {
            auto* entry = layout->getEntryPointByIndex(ep);
            if (entry->getStage() != SLANG_STAGE_VERTEX) {
                continue;
            }

            unsigned vp_count = entry->getParameterCount();
            MGE_DEBUG_TRACE(SLANG,
                            "Vertex entry point has {} parameters",
                            vp_count);

            for (unsigned p = 0; p < vp_count; ++p) {
                auto* vparam = entry->getParameterByIndex(p);
                auto  vcat = vparam->getCategory();

                if (vcat != slang::ParameterCategory::VaryingInput) {
                    continue;
                }

                auto*       vvar = vparam->getVariable();
                const char* attr_name = vvar ? vvar->getName() : nullptr;
                auto*       attr_type = vvar ? vvar->getType() : nullptr;

                if (!attr_type) {
                    continue;
                }

                // Handle struct type (multiple attributes packed in
                // a struct)
                if (attr_type->getKind() ==
                    slang::TypeReflection::Kind::Struct) {
                    unsigned fc = attr_type->getFieldCount();
                    for (unsigned fi = 0; fi < fc; ++fi) {
                        auto*       field = attr_type->getFieldByIndex(fi);
                        auto*       field_type = field->getType();
                        auto        dt = attribute_type_from_slang(field_type);
                        const char* fname = field->getName();

                        MGE_DEBUG_TRACE(SLANG,
                                        "  Vertex attribute '{}': type={}",
                                        fname ? fname : "(null)",
                                        dt);

                        result.attributes.push_back(
                            {fname ? fname : "", dt, 1});
                    }
                } else {
                    auto dt = attribute_type_from_slang(attr_type);

                    MGE_DEBUG_TRACE(SLANG,
                                    "  Vertex attribute '{}': type={}",
                                    attr_name ? attr_name : "(null)",
                                    dt);

                    result.attributes.push_back(
                        {attr_name ? attr_name : "", dt, 1});
                }
            }
        }
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
        session_desc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;

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

        extract_reflection(layout, result);

        return result;
    }

} // namespace mge
