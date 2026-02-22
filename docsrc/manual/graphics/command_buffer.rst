***************
Command Buffers
***************

Command buffers capture state changes on a graphics pipeline.

They are used to record commands that will be sent to the GPU. In a multi-threaded
application, command buffers can be recorded on multiple threads and then submitted
to the GPU on a single thread.

Pipeline State
==============

The pipeline state encapsulates all configuration for the graphics rendering pipeline,
including depth/stencil testing, blending, rasterization, face culling, and other 
rendering parameters.

The ``pipeline_state`` class uses a compact bitfield representation to efficiently
store all state flags and parameters. Command buffers maintain a current pipeline
state that is captured with each draw call.

Key state components controlled by pipeline state:

- **Depth testing**: Comparison function and write enable
- **Blending**: Operations and factors for color/alpha composition
- **Face culling**: Which triangle faces to discard
- **Primitive topology**: Triangle list, strip, line list, or points
- **Color write mask**: Which color channels to write
- **Stencil operations**: Stencil testing and operations
- **Depth bias**: For shadow mapping and Z-fighting prevention

The command buffer provides convenient methods to configure common state changes,
which internally update the current pipeline state.

.. doxygenclass:: mge::pipeline_state
    :members:

Blend State
===========

Command buffers support configuring blend state for transparency and compositing effects.
The blend state determines how source (incoming fragment) and destination (framebuffer)
colors are combined.

Basic blending is controlled by three components:

- **Blend operation**: How source and destination are combined (add, subtract, min, max)
- **Source blend factor**: Weight applied to source color
- **Destination blend factor**: Weight applied to destination color

The final color is computed as::

    result = (src * src_factor) op (dst * dst_factor)

Example usage for standard alpha blending::

    command_buffer cmd;
    cmd.blend_equation(blend_operation::ADD);
    cmd.blend_function(blend_factor::SRC_ALPHA, blend_factor::ONE_MINUS_SRC_ALPHA);
    cmd.draw(program, vertices, indices);

To disable blending::

    cmd.blend_opaque();

Face Culling
============

Face culling is a rendering optimization that discards triangles (faces) based on their
orientation relative to the camera. This is commonly used to avoid rendering the back
faces of solid objects, improving performance.

Culling mode determines which faces are discarded:

- **NONE**: No culling - both front and back faces are rendered (default)
- **CLOCKWISE**: Cull clockwise-facing triangles
- **COUNTER_CLOCKWISE**: Cull counter-clockwise-facing triangles

The winding order of a triangle is determined by the order of its vertices when viewed
from the camera. Typically, counter-clockwise culling is used to hide back faces of
models with counter-clockwise front faces.

Example usage::

    command_buffer cmd;
    cmd.cull_face(cull_mode::COUNTER_CLOCKWISE);  // Cull back faces
    cmd.draw(program, vertices, indices);

To disable culling::

    cmd.cull_face(cull_mode::NONE);

Depth Testing
=============

Depth testing determines which fragments are visible based on their depth values,
enabling proper rendering of 3D scenes with occluded geometry.

The depth test compares the fragment's depth with the depth buffer value using
a comparison function. If the test passes, the fragment is rendered and optionally
the depth buffer is updated.

Example usage::

    command_buffer cmd;
    cmd.depth_test_function(test::LESS);  // Standard depth test
    cmd.depth_write(true);                // Enable depth writes
    cmd.draw(program, vertices, indices);

Example for transparent objects (read depth, don't write)::

    cmd.depth_test_function(test::LESS);
    cmd.depth_write(false);  // Don't write to depth buffer
    cmd.draw(program, vertices, indices);

API Reference
=============

.. doxygenclass:: mge::command_buffer
    :members:

.. doxygenclass:: mge::pipeline_state
    :members:

.. doxygenenum:: mge::blend_operation

.. doxygenenum:: mge::blend_factor

.. doxygenenum:: mge::cull_mode

.. doxygenenum:: mge::test
