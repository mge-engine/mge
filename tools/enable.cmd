@echo off

rem Call this script to enable some properties for MGE
rem Usage: enable.cmd [features]
rem Available features:
rem   opengl          Enable OpenGL render system
rem   vulkan          Enable Vulkan render system
rem   dx11            Enable DirectX11 render system
rem   dx12            Enable DirectX12 render system
rem   trace           Enable trace to console
rem   renderdoc       Enable RenderDoc recording
rem                 
rem Example: enable.cmd opengl trace renderdoc

:parse_args
if "%~1"=="" goto end_parse_args

if /i "%~1"=="opengl" (
    @echo Enable opengl render system
    SET MGE_RENDER_SYSTEM=opengl
    shift
    goto parse_args
)

if /i "%~1"=="vulkan" (
    @echo Enable vulkan render system
    SET MGE_RENDER_SYSTEM=vulkan
    shift
    goto parse_args
)

if /i "%~1"=="dx11" (
    @echo Enable DirectX11 render system
    SET MGE_RENDER_SYSTEM=dx11
    shift
    goto parse_args
)

if /i "%~1"=="dx12" (
    @echo Enable DirectX12 render system
    SET MGE_RENDER_SYSTEM=dx12
    shift
    goto parse_args
)

if /i "%~1"=="trace" (
    @echo Enable trace to console
    SET MGE_TRACE_TO_STDOUT=1
    SET MGE_TRACE_ENABLED=1
    shift
    goto parse_args
)

if /i "%~1"=="renderdoc" (
    @echo Enable RenderDoc support
    SET MGE_RENDERDOC_ENABLED=1
    shift
    goto parse_args
)

@echo Unknown feature: %~1
shift   
goto parse_args
:end_parse_args