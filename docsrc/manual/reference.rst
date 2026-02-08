*********
Reference
*********

This section contains the complete reference documentation for MGE libraries and configurable parameters.

Libraries
=========

.. toctree::
    :maxdepth: 1

    mgecore
    mgemath
    mgegraphics
    mgeinput
    mgeapplication
    mgeasset

Parameters
==========

.. include:: parameters.rst

Example Programs
================

The following sample programs demonstrate various features of MGE:

* **blackscreen** - Minimal application demonstrating basic window creation and render context setup
* **blend** - Demonstrates additive blending with overlapping triangles using command buffer blend state
* **triangle** - Renders a simple triangle using vertex shaders and the graphics pipeline
* **teapot** - Renders a 3D teapot model demonstrating mesh loading and rendering

Sample programs are located in ``src/samples/`` and can be configured via JSON config files.
