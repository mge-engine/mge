@echo off

rem Call this script to disable some properties for MGE
rem Usage: disable.cmd [features]
rem Available features:
rem   trace           Disable trace to console
rem   renderdoc       Disable RenderDoc recording
rem                 
rem Example: disable.cmd trace renderdoc

:parse_args
if "%~1"=="" goto end_parse_args

if /i "%~1"=="trace" (
    @echo Disable trace to console
    SET MGE_TRACE_TO_STDOUT=
    SET MGE_TRACE_ENABLED=
    shift
    goto parse_args
)

if /i "%~1"=="renderdoc" (
    @echo Disable RenderDoc support
    SET MGE_RENDERDOC_ENABLED=
    shift
    goto parse_args
)

@echo Unknown feature: %~1
shift   
goto parse_args
:end_parse_args
