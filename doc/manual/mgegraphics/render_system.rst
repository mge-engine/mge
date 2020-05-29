.. mgegraphics_render_system:

*************
Render System
*************

The *render system* is the abstraction of the render backend and its
entry point. It is defined by the graphics library, but implemented
in modules specific to the respective backend.

The render system gives you access to the following functionality:

* examining the monitors of your system
* creating windows to actually do rendering
* manage render backend specific properties

.. doxygenclass:: mge::render_system
   :project: mge
   :members:

