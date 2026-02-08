***************
Command Buffers
***************

Command buffers capture state changes on a graphics pipeline.

They are used to record commands that will be sent to the GPU. In a multi-threaded
application, command buffers can be recorded on multiple threads and then submitted
to the GPU on a single thread.

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

.. doxygenclass:: mge::command_buffer
    :members:

.. doxygenenum:: mge::blend_operation

.. doxygenenum:: mge::blend_factor

