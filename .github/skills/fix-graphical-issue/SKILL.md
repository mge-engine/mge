---
name: fix-graphical-issue
description: 'Debug and fix graphical rendering issues in MGE using screenshots. Use when: visual bugs, incorrect colors, missing geometry, blank screens, z-fighting, blend artifacts, wrong depth, before/after screenshot comparison, verifying rendering fixes.'
---

# Fix Graphical Issue

## When to Use

- Visual rendering output is incorrect (wrong colors, missing geometry, artifacts)
- Screen is blank or black when it should not be
- Blend, depth, or stencil issues
- Shader produces wrong output
- Need to verify a rendering fix visually with before/after screenshots

## Workflow Overview

1. Take a **before** screenshot (capture the bug)
2. View the screenshot to confirm the problem
3. Diagnose and fix the issue
4. Build
5. Take an **after** screenshot (same frame, same render system)
6. View and compare both screenshots
7. Run backend-specific capture tests: `ctest --output-on-failure --test-dir build -L {render_system}`
8. Run remaining backends to check for regressions: `ctest --output-on-failure --test-dir build`

## Taking a Screenshot

### Via Command Line Parameters

Run the application with screenshot parameters:

```powershell
.\{executable}.exe -c "graphics.screenshot_at_frame=10" -c "application.stop_at_cycle=11" -c "graphics.render_system={render_system}"
```

- `graphics.screenshot_at_frame=N` — capture at frame N (1-based, 0 disables)
- `application.stop_at_cycle=N+1` — stop one cycle after to ensure the screenshot is saved
- `graphics.render_system` — one of: `directx11`, `directx12`, `opengl`, `vulkan`

The screenshot is saved to the working directory as:

```
{executable_name}-{render_system}-{frame}.png
```

Example:

```powershell
cd build
.\blend.exe -c "graphics.screenshot_at_frame=10" -c "application.stop_at_cycle=11" -c "graphics.render_system=opengl"
# Saves: blend-opengl-10.png
```

### Via CMake Screenshot Targets

Pre-configured targets exist for samples:

```powershell
cmake --build build --target {sample}-{render_system}-screenshot
```

These run with `screenshot_at_frame=10` and `stop_at_cycle=11`.

### Via Key Press (Interactive)

In sample applications, press `P` to save a screenshot to `/temp/screenshot.png`.

## Viewing Screenshots

Use the `view_image` tool to inspect screenshot PNG files:

- View the before screenshot to confirm the visual bug
- View the after screenshot to confirm the fix
- Compare both to verify the rendering change

## Before/After Comparison Workflow

### Step 1: Capture Before

```powershell
cd build
.\{app}.exe -c "graphics.screenshot_at_frame=10" -c "application.stop_at_cycle=11" -c "graphics.render_system={rs}"
```

Rename or note the output file as the "before" image:

```powershell
Rename-Item "{app}-{rs}-10.png" "{app}-{rs}-10-before.png"
```

### Step 2: Apply Fix and Build

Make code changes and rebuild:

```powershell
cmake --build build
```

### Step 3: Capture After

```powershell
cd build
.\{app}.exe -c "graphics.screenshot_at_frame=10" -c "application.stop_at_cycle=11" -c "graphics.render_system={rs}"
```

The new file is the "after" image.

### Step 4: Compare

View both images with `view_image` and verify the fix visually.

## Diagnosing Rendering Issues

### Enable Trace Output

```powershell
$env:MGE_TRACE_ENABLED=1
$env:MGE_TRACE_TO_STDOUT=1
```

Or via parameters:

```powershell
.\{app}.exe -c "trace.globally_enabled=true" -c "trace.print_to_stdout=true" -c "trace.GRAPHICS=DEBUG,INFO,WARNING,ERROR"
```

### Per-Backend Trace Topics

| Topic | Backend |
|-------|---------|
| `DX11` | DirectX 11 |
| `DX12` | DirectX 12 |
| `OPENGL` | OpenGL |
| `VULKAN` | Vulkan |
| `GRAPHICS` | Graphics core |
| `SLANG` | Shader compilation |

### Common Causes

| Symptom | Check |
|---------|-------|
| Black/blank screen | Render passes active? Shaders compiled? Correct render system loaded? |
| Wrong colors | Shader output, blend state, texture format, image format (RGBA vs BGRA) |
| Missing geometry | Vertex buffer data, index buffer, draw calls, culling state |
| Z-fighting | Depth buffer format, near/far planes, depth test function |
| Blend artifacts | Blend state, alpha values, render order |
| Flickering | Double buffering, present mode, synchronization |

## Key Source Files

| File | Purpose |
|------|---------|
| `src/mge/graphics/render_context.hpp` | `screenshot()` virtual method, `set_screenshot_at_frame()` |
| `src/mge/graphics/render_context.cpp` | `save_screenshot()` implementation, `graphics.screenshot_at_frame` parameter |
| `src/modules/directx11/render_context.cpp` | DX11 `screenshot()` via staging texture |
| `src/modules/directx12/render_context.cpp` | DX12 `screenshot()` via readback buffer |
| `src/modules/opengl/render_context.cpp` | OpenGL `screenshot()` via `glReadPixels` |
| `src/modules/vulkan/render_context.cpp` | Vulkan `screenshot()` via staging buffer |
| `cmake/modules/macros/test.cmake` | `MGE_CAPTURE_TEST` function, screenshot targets |

## Configuration Parameters Reference

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `graphics.screenshot_at_frame` | uint64_t | 0 | Frame number to capture (1-based, 0 disables) |
| `graphics.render_system` | string | — | Render backend: `directx11`, `directx12`, `opengl`, `vulkan` |
| `application.stop_at_cycle` | uint64_t | 0 | Cycle number to stop application (0 disables) |
| `graphics.record_frames` | bool | false | Enable RenderDoc frame recording |
| `graphics.frame_debugger` | string | — | Frame debugger component (e.g. `renderdoc`) |
