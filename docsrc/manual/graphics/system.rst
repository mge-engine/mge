*************
Render System
*************

The render system is the basic entry point into the graphics library.

The default render system can be selected by
    - setting the environment variable :envvar:`MGE_RENDER_SYSTEM`, or
    - setting the configuration parameter `graphics.render_system`.

.. envvar:: MGE_RENDER_SYSTEM

   Specifies the render system implementation to be used, such as "opengl"
   or "vulkan".

.. doxygenclass:: mge::render_system
    :members:

