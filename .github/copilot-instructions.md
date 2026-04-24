ALWAYS ANSWER IN SHORTEST POSSIBLE WAY THAT IS STILL COMPLETE AND CLEAR
AND USES THE FEWEST TOKENS POSSIBLE.

# Project Overview 

* MGE (Modern Game/Graphics Engine) is a framework having the following goals:
* cross-platform, C++20
* modular (plugins)

## General Rules

* do not optimize
* do not abstract
* do not generalize
* do not omit steps in reasoning
* do not defer decisions
* verify more, assume less, and communicate uncertainty clearly

* Always execute 'cmake --build build' to build the project, do a build always to verify a change
* When checking an error, use 'cmake --build build -j 1' to do a single-threaded build for easier debugging
* After building, run the appropriate tests (see Test Workflow below)
* Run each test command exactly once for verification. Do not re-run the same tests multiple times.
* Execute these commands from the root of the repository.

# Commit Messages

* Use present tense ("Add feature" not "Added feature")
* Be concise yet descriptive
* Use the following structure:
    - A short summary of the change (50 characters or less)
    - A blank line
    - A detailed description of the change, if necessary (wrap at 72 characters)
* Do not mention that test passes in the commit message unless the commit is specifically about fixing or adding tests.

## Communication

* be explicit
* short statements
* no human simulation

## Architecture

### Core-Module Pattern

- core libraries (`src/mge/`): abstractions for graphics, application, input, math, asset mgmt, scenes, etc.
- modules (`src/modules/`): modules register as `component` implementations at runtime
- 

### Key Abstractions
- Reference types: `MGE_DECLARE_REF(type)` macro generates `type_ref` as `std::shared_ptr<type>`
- **Component system**: Plugin architecture (see `src/mge/core/component.hpp`)
- **Tracing**: Structured logging via `MGE_USE_TRACE(TOPIC)`, `MGE_DEBUG_TRACE(TOPIC, message)`, `MGE_INFO_TRACE(TOPIC, message)`
  - Define topics per subsystem (DX11, OPENGL, CORE, etc.)
  - Enable at runtime: `$env:MGE_TRACE_ENABLED=1; $env:MGE_TRACE_TO_STDOUT=1`

### Dependencies
  - use vcpkg ([vcpkg.json](../vcpkg.json))

## Build & Test Workflow

### Standard Build
- Invoke in project directory:
```powershell
cmake --build build          # Multi-threaded build
```
- if build does not do anything, assume all fine (user did build it)

### Test Workflow

Tests are labeled with CTest labels: `unit`, `graphics`, `capture`, `lua`, `opengl`, `directx11`, `directx12`, `vulkan`.

#### Test Commands
```powershell
# Quick tests (all except capture/ref protocol tests)
cmake --build build --target quick-tests

# All tests including capture/ref protocol
ctest --output-on-failure --test-dir build

# Backend-specific capture tests only
ctest --output-on-failure --test-dir build -L opengl
ctest --output-on-failure --test-dir build -L directx11
ctest --output-on-failure --test-dir build -L directx12
ctest --output-on-failure --test-dir build -L vulkan

# Lua tests only
ctest --output-on-failure --test-dir build -L lua

# Unit tests only (no graphics, no capture)
ctest --output-on-failure --test-dir build -L unit
```

#### Which Tests to Run (Agent Guidance)

Choose tests based on which files were changed:

| Changed files | Tests to run |
|---------------|-------------|
| `src/mge/core/`, `src/mge/math/`, `src/mge/reflection/`, `src/mge/asset/`, `src/mge/input/` | `quick-tests` |
| `src/modules/lua/` | `ctest -L lua` first, then `quick-tests` |
| `src/modules/directx11/` | `ctest -L directx11` first, then other backends |
| `src/modules/directx12/` | `ctest -L directx12` first, then other backends |
| `src/modules/opengl/` | `ctest -L opengl` first, then other backends |
| `src/modules/vulkan/` | `ctest -L vulkan` first, then other backends |
| `src/mge/graphics/`, `src/mge/ui/`, `assets/shaders/`, `src/samples/` | All tests: `ctest --output-on-failure --test-dir build` |
| `cmake/`, `CMakeLists.txt`, build system changes | All tests: `ctest --output-on-failure --test-dir build` |

When fixing a backend-specific issue, run that backend's capture tests first. If they pass, run other backends to check for regressions.
  

### CMake Structure
- use CMakePresets.json for settings
- **Custom macros**: [cmake/modules/macros/](../cmake/modules/macros/)
  - `MGE_LIBRARY()`: library with auto-defined `BUILD_<TARGET>` flags
  - `MGE_TEST()`: Google Test integration with display/headless env handling
  - Tests requiring display use `NEEDSDISPLAY` flag (see [../cmake/modules/macros/test.cmake](../cmake/modules/macros/test.cmake))

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
Enforced by [.clang-format](../.clang-format):
- 4 spaces, no tabs
- Pointer/reference left-aligned: `Type* ptr`, `Type& ref`
- Braces: Allman style for classes/functions, K&R for control flow
- Constructor initializers: break before comma (`,` starts continuation lines)
- 80-character line limit

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
- Use structured tracing instead of `std::cout`:

### Testing
- Unit tests use Google Test via `MGE_TEST()` macro in `src/test/`
- Test structure: `src/test/<component>/test_<feature>.cpp`
- Example: `TEST(benchmark, memory_cycle) { ... }` in [src/test/test/test_benchmark.cpp](../src/test/test/test_benchmark.cpp)

### Sample Tests
- in `src/samples` directory
- demonstrate module usage and integration
- full applications
- configured via json config when running

## Common Pitfalls
- Don't create in-source builds (CMake rejects them)
- Module loading depends on `MGE_RENDER_SYSTEM` environment variable
- DirectX modules are Windows-only; OpenGL/Vulkan are cross-platform
- Headless tests skip `NEEDSDISPLAY` tests automatically (CI environments)
