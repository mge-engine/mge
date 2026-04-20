---
name: regenerate-capture-test
description: 'Regenerate capture test reference files after intentional rendering changes. Use when: capture tests fail because shaders, draw calls, uniform buffers, or render state changed intentionally; updating reference .md files in src/test/graphics/; running capture-* CMake targets.'
---

# Regenerate Capture Test

## When to Use

- Capture tests fail after an intentional rendering change (new shaders, changed uniforms, added draw calls, modified render state)
- Reference files in `src/test/graphics/` need updating to match new GPU API call sequences
- A new sample with `MGE_CAPTURE_TEST` was added and needs initial reference files

## How Capture Tests Work

Each sample with `MGE_CAPTURE_TEST` in its CMakeLists.txt gets:

1. **CTest tests** `test_{target}_capture_{render_system}` — run the sample under RenderDoc, capture GPU API calls, sanitize to a deterministic markdown format, and compare against reference files in `src/test/graphics/{target}_{render_system}.md`
2. **CMake targets** `capture-{target}-{render_system}` — same as above but writes the sanitized output as the new reference file (generate mode)

The workflow (implemented in `capturetest.py`):
1. Run the sample executable with RenderDoc frame capture (5 frames)
2. Convert the `.rdc` capture to XML via `renderdoccmd convert`
3. Sanitize the XML: strip RenderDoc internals, normalize resource IDs, produce deterministic markdown
4. Compare against (or overwrite) the reference file in `src/test/graphics/`

## Reference File Location

```
src/test/graphics/{target}_{render_system}.md
```

Examples:
- `src/test/graphics/teapot_opengl.md`
- `src/test/graphics/triangle_directx11.md`
- `src/test/graphics/blend_vulkan.md`

## Regenerating References

### Single backend

```powershell
cmake --build build --target capture-{target}-{render_system}
```

Examples:
```powershell
cmake --build build --target capture-teapot-opengl
cmake --build build --target capture-teapot-directx11
cmake --build build --target capture-teapot-directx12
cmake --build build --target capture-teapot-vulkan
```

### All backends for one sample

Run each backend target:
```powershell
cmake --build build --target capture-{target}-opengl
cmake --build build --target capture-{target}-directx11
cmake --build build --target capture-{target}-directx12
cmake --build build --target capture-{target}-vulkan
```

## Available Capture Targets

| Sample | Targets |
|--------|---------|
| blackscreen | `capture-blackscreen-{rs}` |
| blend | `capture-blend-{rs}` |
| depth_test | `capture-depth_test-{rs}` |
| hello_ui | `capture-hello_ui-{rs}` |
| rotating_triangle | `capture-rotating_triangle-{rs}` |
| teapot | `capture-teapot-{rs}` |
| textured_triangle | `capture-textured_triangle-{rs}` |
| triangle | `capture-triangle-{rs}` |

Where `{rs}` is one of: `opengl`, `directx11`, `directx12`, `vulkan`.

## Workflow

1. Make the rendering change
2. Build: `cmake --build build`
3. Run tests to confirm which capture tests fail: `ctest --output-on-failure --test-dir build`
4. Regenerate reference files for each failing backend: `cmake --build build --target capture-{target}-{render_system}`
5. Run tests again to verify all pass: `ctest --output-on-failure --test-dir build`
6. Commit the updated reference files along with the rendering change
