# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

IF(MSVC)
    ADD_DEFINITIONS("-FC")
    ADD_DEFINITIONS("-D_CRT_SECURE_NO_WARNINGS -Wall")
    ADD_DEFINITIONS("-D_SCL_SECURE_NO_WARNINGS")
    ADD_DEFINITIONS("-fp:fast")
    ADD_DEFINITIONS("-WX")
    ADD_DEFINITIONS("-Qpar")
    ADD_DEFINITIONS("-Zc:preprocessor")
    ADD_DEFINITIONS("-DNOMINMAX") # avoid window.h defined min/max
    ADD_DEFINITIONS("-wd4702") # unreachable code (happens in template expansion)
    ADD_DEFINITIONS("-wd4710") # cannot inline function marked as inline
    ADD_DEFINITIONS("-wd4514") # unreferenced inline function removed
    ADD_DEFINITIONS("-wd4365") # unsigned/signed conflict
    ADD_DEFINITIONS("-wd4355") # this is used in initialization list
    ADD_DEFINITIONS("-wd4371") # layout has changed due to better packing
    ADD_DEFINITIONS("-wd4571") # catch semantics changed for catch(...)
    ADD_DEFINITIONS("-wd4774") # format string is no format literal
    ADD_DEFINITIONS("-wd4623") # deleted default constructor
    ADD_DEFINITIONS("-wd5026") # deleted move constructor
    ADD_DEFINITIONS("-wd5027") # deleted move assigment
    ADD_DEFINITIONS("-wd4625") # deleted copy constructor
    ADD_DEFINITIONS("-wd4626") # deleted assignment operator
    ADD_DEFINITIONS("-wd4820") # added alignment/padding bytes
    ADD_DEFINITIONS("-wd4275") # std::exception has no DLL interface
    ADD_DEFINITIONS("-wd4668") # #if undefined is #if 0
    ADD_DEFINITIONS("-wd4987") # throw(...) used
    ADD_DEFINITIONS("-wd4251") # std::string has no DLL interface
    ADD_DEFINITIONS("-wd4100") # unreferenced formal parameter
    ADD_DEFINITIONS("-wd4619") # unknown warning number (caused by boost)
    ADD_DEFINITIONS("-wd4868") # compiler does not guarantee evaluation order (caused by ranges)
    ADD_DEFINITIONS("-wd5031") # warning state moved by push into other file (caused by boost)
    ADD_DEFINITIONS("-wd5032") # bad pragma warning push (boost)
    ADD_DEFINITIONS("-wd4555") # expression has no side effect (caused by boost)
    ADD_DEFINITIONS("-wd4061") # enumerator not handled in case (caused by xatomic ms header)
    ADD_DEFINITIONS("-wd4435") # object layout changed because of virtual base class (always issued by vc)
    ADD_DEFINITIONS("-wd4595") # new/delete must not be inline
    ADD_DEFINITIONS("-wd4643") # forwarding in namespace (boost)
    ADD_DEFINITIONS("-wd5039") # vc TpSetCallbackCleanupGroup internally raised issue (extern "C" and exceptions)
    ADD_DEFINITIONS("-wd5045") # Spectre
    ADD_DEFINITIONS("-wd4255") # () -> (void) in winuser.h
    ADD_DEFINITIONS("-wd4582") # constructor will not be implicitly called
    ADD_DEFINITIONS("-wd5219") # possible data loss in conversion
    ADD_DEFINITIONS("-wd5243") # 'type-name': using incomplete class 'class-name' can cause potential one definition rule violation due to ABI limitation
    ADD_DEFINITIONS("-wd4686") # possible change in behavior, change in UDT return calling convention
    ADD_DEFINITIONS("-wd5262") # implicit fallthrough
    ADD_DEFINITIONS("-wd5264") # variable not used
    ADD_DEFINITIONS("-wd4883") # function size supresses optimizations
    ADD_DEFINITIONS("-wd4189") # local variable is initialized but never referenced
    ADD_DEFINITIONS("-showIncludes")
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
        ADD_DEFINITIONS("-wd4548")
        SET(NODEFAULTLIB "-NODEFAULTLIB:libcmt.lib -NODEFAULTLIB:libc.lib -NODEFAULTLIB:libcd.lib -NODEFAULTLIB:libcmtd.lib -NODEFAULTLIB:msvcrt.lib")
    ELSE()
        SET(NODEFAULTLIB "-NODEFAULTLIB:libcmt.lib -NODEFAULTLIB:libc.lib -NODEFAULTLIB:libcd.lib -NODEFAULTLIB:libcmtd.lib -NODEFAULTLIB:msvcrtd.lib")
    ENDIF()
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${NODEFAULTLIB}")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${NODEFAULTLIB}")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${NODEFAULTLIB}")
ENDIF()
