# Project Overview

MGE (Modern Game/Graphics Engine) is a cross-platform graphics engine written in C++20. The project demonstrates modern C++ patterns combined with real-time graphics, supporting multiple rendering backends (DirectX 11, DirectX 12, OpenGL, Vulkan) through a plugin-based module system.

## Architecture

### Core-Module Pattern
- **Core libraries** (`src/mge/`): Platform-independent abstractions for graphics, application, input, math, asset management
- **Backend modules** (`src/modules/`): Rendering implementations loaded as shared libraries (`.dll`/`.so`)
  - Modules register as `component` implementations at runtime (see `src/mge/core/component.hpp`)
  - Switch backends via environment variable: `$env:MGE_RENDER_SYSTEM = "dx11"` (or `vulkan`, `dx12`, `opengl`)
  - Scripts in `tools/` (e.g., [enable_dx11.ps1](tools/enable_dx11.ps1), [reset_render_system.ps1](tools/reset_render_system.ps1))

### Key Abstractions
- **Reference types**: Use `MGE_DECLARE_REF(type)` macro to generate `type_ref` as `std::shared_ptr<type>`
- **Component system**: Plugin architecture for render systems, asset loaders, etc. (see `src/mge/core/component.hpp`)
- **Tracing**: Structured logging via `MGE_USE_TRACE(TOPIC)`, `MGE_DEBUG_TRACE(TOPIC, message)`, `MGE_INFO_TRACE(TOPIC, message)`
  - Define topics per subsystem (DX11, OPENGL, CORE, etc.)
  - Enable at runtime: `$env:MGE_TRACE_ENABLED=1; $env:MGE_TRACE_TO_STDOUT=1`

### Dependencies
Managed via vcpkg ([vcpkg.json](vcpkg.json)): GLM, Assimp, Boost, STB, glslang, Vulkan Memory Allocator, etc.

## Build & Test Workflow

### Standard Build
```powershell
cmake --build build          # Multi-threaded build
cmake --build build -j 1     # Single-threaded for debugging errors
ctest --output-on-failure --test-dir build
```

### CMake Structure
- **Presets**: [CMakePresets.json](CMakePresets.json) defines `default` (local dev) and `ci-build` (headless CI)
- **Custom macros**: [cmake/modules/macros/](cmake/modules/macros/)
  - `MGE_LIBRARY()`: Creates libraries with auto-defined `BUILD_<TARGET>` flags
  - `MGE_TEST()`: Google Test integration with display/headless environment handling
  - Tests requiring display use `NEEDSDISPLAY` flag (see [cmake/modules/macros/test.cmake](cmake/modules/macros/test.cmake))

### Module Development
Modules are built as `MODULE` or `SHARED` libraries:
```cmake
ADD_LIBRARY(mge_module_dx11 MODULE render_context.cpp ...)
TARGET_LINK_LIBRARIES(mge_module_dx11 mgecore mgegraphics ...)
```
They export component implementations discoverable at runtime.

## Coding Standards

### Naming Conventions
- **snake_case** for everything: classes, methods, variables
- **UPPER_CASE** for constants, enum values
- Methods are verbs; classes are nouns
- **No** `get_`/`is_` prefixes for accessors—just the property name (e.g., `name()`, not `get_name()`)
- Avoid common suffixes like `_type` (use C++ standard library naming where applicable)

### Code Style
Enforced by [.clang-format](.clang-format):
- 4 spaces, no tabs
- Pointer/reference left-aligned: `Type* ptr`, `Type& ref`
- Braces: Allman style for classes/functions, K&R for control flow
- Constructor initializers: break before comma (`,` starts continuation lines)
- 80-character line limit
- See [doc/design/ProjectStructure.md](doc/design/ProjectStructure.md) for detailed conventions

### Namespaces
- All code in `namespace mge`
- Backend-specific code in nested namespaces: `mge::dx11`, `mge::opengl`, `mge::vulkan`
- Internal details in `detail` namespace or anonymous namespace

### Error Handling & Tracing
Always declare trace topics at file scope:
```cpp
namespace mge {
    MGE_USE_TRACE(DX11);  // Declare once per file
}
```
Use structured tracing instead of `std::cout`:
```cpp
MGE_DEBUG_TRACE(DX11, "Creating render context");
MGE_INFO_TRACE(OPENGL, "Frame recording is {}", enabled ? "enabled" : "disabled");
```

### Testing
- Unit tests use Google Test via `MGE_TEST()` macro in `src/test/`
- Test structure: `src/test/<component>/test_<feature>.cpp`
- Example: `TEST(benchmark, memory_cycle) { ... }` in [src/test/test/test_benchmark.cpp](src/test/test/test_benchmark.cpp)

## Common Pitfalls
- Don't create in-source builds (CMake rejects them)
- Module loading depends on `MGE_RENDER_SYSTEM` environment variable
- DirectX modules are Windows-only; OpenGL/Vulkan are cross-platform
- Headless tests skip `NEEDSDISPLAY` tests automatically (CI environments)
