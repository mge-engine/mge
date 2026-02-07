***************
Shader Programs
***************

A shader program is an user-defined program which is executed as part of a
render pipeline on a graphics card.

Types and Definitions
=====================

Shader Types
------------

.. doxygenenum:: mge::shader_type

Shader Language
---------------

Shader languages are used to write shader source code. Examples include GLSL and HLSL.

.. doxygenclass:: mge::shader_language
    :members:

Shader Format
-------------

Shader formats define the binary format of compiled shaders. Examples include SPIR-V and DXBC.

.. doxygenclass:: mge::shader_format
    :members:

Shader
------

.. doxygenclass:: mge::shader
    :members:

Program
-------

A program combines multiple shader stages into a complete rendering pipeline.

.. doxygenclass:: mge::program
    :members:
