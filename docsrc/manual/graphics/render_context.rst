**************
Render Context
**************

Render APIs use a context object that combines objects of the render
pipeline. A render context is btained from a :any:`mge::window`, and
has a 1:1 relationship with the window.

.. doxygenclass:: mge::render_context
    :members:

Context objects are created based upon a render context. Their life time
is bound to the render context, and they are valid for one render context
only.

.. doxygenclass:: mge::context_object
    :members:

Each render context has a swap chain, a series of buffers to stabilize frame
rates and enable presenting frames.

.. doxygenclass:: mge::swap_chain
    :members:

Render Pass
===========

A pass organizes rendering commands with specific state settings for clear
operations, viewports, and scissor rectangles.

.. doxygenclass:: mge::pass
    :members:

Frame Buffer
============

A frame buffer is collection of render targets for rendering operations.

.. doxygenclass:: mge::frame_buffer
    :members:
