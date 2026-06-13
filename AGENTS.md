# Agent Instructions

## General Rules

### Think before code

Don't assume. Don't hide confusion. Surface tradeoffs.

* state assumptions, ask user in doubt
* if multiple intepretations exist, present them and ask    
* if simpler approach exist, say so
* if something is unclear, stop and ask 
* do not defer decisions
* use short statements

### Simplicity

Minimum code that solves the problem. Nothing speculative.

* No features beyond what was asked.
* No abstractions for single-use code.
* No "flexibility" or "configurability" that wasn't requested.
* No error handling for impossible scenarios.
* If you write 200 lines and it could be 50, rewrite it.

Ask yourself: "Would a senior engineer say this is overcomplicated?" If yes, simplify.

### Surgical Change

Touch only what you must. Clean up only your own mess.

When editing existing code:

* Don't "improve" adjacent code, comments, or formatting.
* Don't refactor things that aren't broken.
* Match existing style, even if you'd do it differently.
* If you notice unrelated dead code, mention it - don't delete it.

When your changes create orphans:

* Remove imports/variables/functions that YOUR changes made unused.
* Don't remove pre-existing dead code unless asked.
* Don't remove comments

The test: Every changed line should trace directly to the user's request.

### Goal-Driven Execution
Define success criteria. Loop until verified.

Transform tasks into verifiable goals:

* "Add validation" → "Write tests for invalid inputs, then make them pass"
* "Fix the bug" → "Write a test that reproduces it, then make it pass"
* "Refactor X" → "Ensure tests pass before and after"

For multi-step tasks, state a brief plan:

1. [Step] → verify: [check]
2. [Step] → verify: [check]
3. [Step] → verify: [check]

Strong success criteria let you loop independently. 
Weak criteria ("make it work") require constant clarification.

## Verify Change

* Only verify when asked in prompt. Do not verify every change by default.
* Execute 'cmake --build build' to build the project,
* After building, run the appropriate tests (see Test Workflow below)
* Run each test command exactly once for verification. Do not re-run the same tests multiple times.
* Execute these commands from the root of the repository.

## Commit Messages

* Use present tense ("Add feature" not "Added feature")
* Be concise yet descriptive.
* Use one line.
* Do not add Co-authored-by: Copilot <copilot@github.com>

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

* quick-tests: changes in core, math, relection, asset, input, lua, python
* directx11, directx12, opengl, vulkan: if one of these backends or graphics changed
* otherwise all tests

When fixing a backend-specific issue, run that backend's capture tests first. If they pass, run other backends to check for regressions.
  

* Core libraries: `src/mge/` — abstractions for graphics, application, input, math, assets, scenes
* Modules: `src/modules/` — register as `component` implementations at runtime
* Reference types: `MGE_DECLARE_REF(type)` generates `type_ref` as `std::shared_ptr<type>`
* Component system: Plugin architecture (`src/mge/core/component.hpp`)
* Dependencies via vcpkg (`vcpkg.json`)

## Coding Standards

* **snake_case** for everything: classes, methods, variables
* **UPPER_CASE** for constants, enum values
* No `get_`/`is_` prefixes for accessors — just the property name
* 4 spaces, no tabs; 80-char line limit
* Pointer/reference left-aligned: `Type* ptr`, `Type& ref`
* Braces: Allman for classes/functions, K&R for control flow
* All code in `namespace mge`; backends in `mge::dx11`, `mge::dx12`, `mge::opengl`, `mge::vulkan`

## Commit Messages

* prefix 1st line with purpose: `feat:`, `fix:`, `refactor:`,`chore:` 
* short summary only
t passes unless the commit is specifically about tests

## Tracing

Structured logging via trace macros:

### Error Handling & Tracing
Always declare trace topics at file scope:
```cpp
MGE_USE_TRACE(TOPIC);           // use existing topic
MGE_DEFINE_TRACE(TOPIC);        // define new topic
MGE_DEBUG_TRACE(TOPIC, msg, args ...); // fmt like trace formatting
```
Macros for DEBUG, INFO, WARNING and ERROR exist.

### Testing
- Unit tests use Google Test via `MGE_TEST()` macro in `src/test/`
- Test structure: `src/test/<component>/test_<feature>.cpp`
- Example: `TEST(benchmark, memory_cycle) { ... }` in [src/test/test/test_benchmark.cpp](../src/test/test/test_benchmark.cpp)

```bash
export MGE_TRACE_ENABLED=1
export MGE_TRACE_TO_STDOUT=1
```
Or: `-c "trace.globally_enabled=true" -c "trace.print_to_stdout=true"`

## Skills Reference

For domain-specific procedures, read these files:

- **Crash debugging**: `.github/skills/fix-crash/SKILL.md` — crash dumps, trace topics, stack traces, debugging procedures
- **Graphical issues**: `.github/skills/fix-graphical-issue/SKILL.md` — screenshots, before/after comparison, rendering diagnosis
- **Regenerate capture tests**: `.github/skills/regenerate-capture-test/SKILL.md` — update reference files after intentional rendering changes

## Common Pitfalls
- Don't create in-source builds (CMake rejects them)
- Graphics module loading depends on `MGE_RENDER_SYSTEM` environment variable
- DirectX modules are Windows-only; OpenGL/Vulkan are cross-platform
- Headless tests skip `NEEDSDISPLAY` tests automatically (CI environments)
