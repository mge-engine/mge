@SET VCPKG_CLONE_DIR="%1\vcpkg"
@SET VCPKG_CMAKE_TOOLCHAIN_FILE="%1\vcpkg\scripts\buildsystems\vcpkg.cmake"
rem BUILD_BINARIESDIRECTORY

%VCPKG_CLONE_DIR%\vcpkg.exe env --triplet x64-windows

cl /help