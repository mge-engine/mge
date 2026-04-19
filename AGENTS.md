# MGE Codex Agent Instructions

## General Rules

- Do not optimize, abstract, or generalize
- Do not omit steps in reasoning
- Do not defer decisions
- Verify more, assume less, communicate uncertainty clearly
- Be explicit, use short statements
- Always build to verify a change: `cmake --build build`
- When checking an error: `cmake --build build -j 1`
- After building, run appropriate tests (see below)
- Run each test command exactly once for verification. Do not re-run the same tests multiple times.
- Execute commands from the repository root

## Build & Test

### Build

```bash
cmake --build build
```

### Test Labels

Tests use CTest labels: `unit`, `graphics`, `capture`, `lua`, `opengl`, `directx11`, `directx12`, `vulkan`.

```bash
# Quick tests (all except capture/ref protocol)
cmake --build build --target quick-tests

# All tests
ctest --output-on-failure --test-dir build

# Backend-specific
ctest --output-on-failure --test-dir build -L opengl
ctest --output-on-failure --test-dir build -L directx11
ctest --output-on-failure --test-dir build -L directx12
ctest --output-on-failure --test-dir build -L vulkan

# Lua only
ctest --output-on-failure --test-dir build -L lua

# Unit only
ctest --output-on-failure --test-dir build -L unit
```

### Which Tests to Run

| Changed files | Tests |
|---|---|
| `src/mge/core/`, `src/mge/math/`, `src/mge/reflection/`, `src/mge/asset/`, `src/mge/input/` | `quick-tests` |
| `src/modules/lua/` | `-L lua`, then `quick-tests` |
| `src/modules/directx11/` | `-L directx11`, then other backends |
| `src/modules/directx12/` | `-L directx12`, then other backends |
| `src/modules/opengl/` | `-L opengl`, then other backends |
| `src/modules/vulkan/` | `-L vulkan`, then other backends |
| `src/mge/graphics/`, `src/mge/ui/`, `assets/shaders/`, `src/samples/` | All tests |
| `cmake/`, `CMakeLists.txt` | All tests |

## Architecture

- Core libraries: `src/mge/` — abstractions for graphics, application, input, math, assets, scenes
- Modules: `src/modules/` — register as `component` implementations at runtime
- Reference types: `MGE_DECLARE_REF(type)` generates `type_ref` as `std::shared_ptr<type>`
- Component system: Plugin architecture (`src/mge/core/component.hpp`)
- Dependencies via vcpkg (`vcpkg.json`)

## Coding Standards

- **snake_case** for everything: classes, methods, variables
- **UPPER_CASE** for constants, enum values
- No `get_`/`is_` prefixes for accessors — just the property name
- 4 spaces, no tabs; 80-char line limit
- Pointer/reference left-aligned: `Type* ptr`, `Type& ref`
- Braces: Allman for classes/functions, K&R for control flow
- All code in `namespace mge`; backends in `mge::dx11`, `mge::opengl`, `mge::vulkan`

## Commit Messages

- Present tense ("Add feature" not "Added feature")
- Short summary (50 chars or less), blank line, detailed description if needed (wrap at 72)
- Do not mention test passes unless the commit is specifically about tests

## Tracing

Structured logging via trace macros:

```cpp
MGE_USE_TRACE(TOPIC);           // use existing topic
MGE_DEFINE_TRACE(TOPIC);        // define new topic
MGE_DEBUG_TRACE(TOPIC) << "msg";
MGE_INFO_TRACE(TOPIC) << "msg";
MGE_WARNING_TRACE(TOPIC) << "msg";
MGE_ERROR_TRACE(TOPIC) << "msg";
```

Enable at runtime:

```bash
export MGE_TRACE_ENABLED=1
export MGE_TRACE_TO_STDOUT=1
```

Or via parameters: `-c "trace.globally_enabled=true" -c "trace.print_to_stdout=true"`

## Skills Reference

For domain-specific procedures, read these files:

- **Crash debugging**: `.github/skills/fix-crash/SKILL.md` — crash dumps, trace topics, stack traces, debugging procedures
- **Graphical issues**: `.github/skills/fix-graphical-issue/SKILL.md` — screenshots, before/after comparison, rendering diagnosis

## Common Pitfalls

- No in-source builds (CMake rejects them)
- Module loading depends on `MGE_RENDER_SYSTEM` environment variable
- DirectX modules are Windows-only; OpenGL/Vulkan are cross-platform
- Headless tests skip `NEEDSDISPLAY` tests automatically
