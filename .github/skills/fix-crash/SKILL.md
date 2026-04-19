---
name: fix-crash
description: 'Debug and fix application crashes in MGE. Use when: analyzing crash dumps, enabling trace output, reading stacktraces, investigating SEH exceptions, diagnosing segfaults, debugging rendering crashes, adding trace instrumentation.'
---

# Fix Crash

## When to Use

- Application crashes or aborts unexpectedly
- A crash dump markdown file was generated
- Need to enable or read trace output
- Need to add trace instrumentation to narrow down a crash
- Investigating an unhandled exception or access violation

## Crash Dump Files

MGE writes crash dumps as markdown files named `{executable}-crash-{timestamp}.md` in the working directory.

A dump contains:
- Timestamp (local and UTC)
- Executable name
- Stack trace with frames: address, module, function, file, line
- Sections from `dump_info_provider` components (trace config, graphics state, etc.)

### Reading a Crash Dump

1. Open the `.md` crash dump file
2. Look at the **stack trace** section — identify the top frames that are in MGE code (not system libraries)
3. Check the **source location** (file + line) of the crashing frame
4. Review **dump_info_provider sections** for additional context (trace state, graphics state)

## Enabling Trace

### Via Environment Variables

```powershell
$env:MGE_TRACE_ENABLED=1
$env:MGE_TRACE_TO_STDOUT=1
```

Both must be set to see trace output on the console.

### Via Configuration Parameters

In a JSON config file or `-c` command line arguments:

```
-c "trace.globally_enabled=true" -c "trace.print_to_stdout=true"
```

### Per-Topic Trace Levels

Each topic can be configured individually via the parameter system:

```
-c "trace.DX11=DEBUG,INFO,WARNING,ERROR"
```

Available levels: `DEBUG`, `INFO`, `WARNING`, `ERROR`, `FATAL`.

## Trace Topics

Core topics defined in the engine:

| Topic | Subsystem | Source |
|-------|-----------|--------|
| `MGE` | Global trace topic | `src/mge/core/trace_topic.cpp` |
| `CORE` | Core library | `src/mge/core/trace_topic.cpp` |
| `CONFIG` | Configuration | `src/mge/core/configuration.cpp` |
| `DUMP` | Crash dump system | `src/mge/core/dump.cpp` |
| `WIN32` | Windows platform | `src/mge/core/platform.cpp` |
| `APPLICATION` | Application lifecycle | `src/mge/application/application.cpp` |
| `ASSET` | Asset loading | `src/mge/asset/asset.cpp` |
| `FILE_ASSET` | File asset source | `src/mge/asset/file_asset_source.cpp` |
| `GRAPHICS` | Graphics/render system | `src/mge/graphics/render_system.cpp` |
| `DX11` | DirectX 11 | `src/modules/directx11/common.cpp` |
| `DX12` | DirectX 12 | `src/modules/directx12/common.cpp` |
| `OPENGL` | OpenGL | `src/modules/opengl/common.cpp` |
| `VULKAN` | Vulkan | `src/modules/vulkan/common.cpp` |
| `GLFW` | GLFW windowing | `src/mge/glfw/window.cpp` |
| `LUA` | Lua scripting | `src/modules/lua/lua_engine.cpp` |
| `SCRIPT` | Script engine | `src/mge/script/script_engine.cpp` |
| `REFLECTION` | Reflection system | `src/mge/reflection/module.cpp` |
| `SLANG` | Slang shader compiler | `src/mge/slang/slang_compiler.cpp` |
| `UI` | UI system | `src/mge/ui/common.cpp` |
| `RENDERDOC` | RenderDoc integration | `src/modules/renderdoc/renderdoc.cpp` |
| `ASSIMP` | Assimp model loading | `src/modules/assimp/assimp.cpp` |

## Adding Trace to Code

### In a `.cpp` file that defines the topic

```cpp
#include "mge/core/trace.hpp"

namespace mge {
    MGE_DEFINE_TRACE(MYTOPIC);
    // ...
    MGE_DEBUG_TRACE(MYTOPIC) << "value is " << x;
    MGE_INFO_TRACE(MYTOPIC) << "initialized";
}
```

### In a file that uses an already-defined topic

```cpp
#include "mge/core/trace.hpp"

namespace mge {
    MGE_USE_TRACE(CORE);
    // ...
    MGE_WARNING_TRACE(CORE) << "something unexpected";
}
```

### Using a topic from another library

```cpp
MGE_USE_IMPORTED_TRACE(DX11);
```

## Procedure

1. **Reproduce** — run the application and trigger the crash
2. **Read the crash dump** — open the generated `*-crash-*.md` file
3. **Identify the crash location** — find the top frame in MGE source code
4. **Enable trace** — set `MGE_TRACE_ENABLED=1` and `MGE_TRACE_TO_STDOUT=1`, then reproduce
5. **Add targeted trace** — if trace output is insufficient, add `MGE_DEBUG_TRACE` calls near the crash site
6. **Check the source** — read the code at the crash location, look for:
   - Null pointer dereference (check pointer/ref validity)
   - Use after free (check object lifetime)
   - Out of bounds access (check container sizes)
   - Uninitialized state (check initialization order)
   - Graphics API errors (check return codes, validation layers)
7. **Fix and verify** — apply the fix, build with `cmake --build build`, run appropriate tests:
   - Non-graphical crash: `cmake --build build --target quick-tests`
   - Graphical/rendering crash: `ctest --output-on-failure --test-dir build -L {backend}` (e.g. `-L opengl`), then `ctest --output-on-failure --test-dir build`

## Key Source Files

| File | Purpose |
|------|---------|
| `src/mge/core/crash.hpp` | `crash()` — terminate with dump |
| `src/mge/core/dump.hpp` | `dump` class — crash dump generation |
| `src/mge/core/dump_info_provider.hpp` | Component interface for dump sections |
| `src/mge/core/stacktrace.hpp` | Stack trace capture |
| `src/mge/core/trace.hpp` | Trace object |
| `src/mge/core/trace_topic.hpp` | Trace topics, macros |
| `src/mge/core/trace_level.hpp` | Trace levels enum |
| `src/mge/core/debugging.hpp` | `breakpoint()`, `is_debugger_present()` |

## Crash Handler

- Installed by `dump::install_handler()` during `application::initialize()`
- **Windows**: SEH via `SetUnhandledExceptionFilter` — captures exception context and writes dump
- **Linux/macOS**: No automatic crash handler yet — use debugger or core dumps

## Debugging Helpers

- `mge::breakpoint()` — triggers debugger break
- `mge::is_debugger_present()` — checks for attached debugger
- `mge::breakpoint_if_debugging()` — breaks only if debugger is attached
