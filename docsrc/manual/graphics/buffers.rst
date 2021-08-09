*******
Buffers
*******

Buffers are collections of data elements. These data elements define
geometry data, offsets into geometry data, or input to shader programs.

A buffer is a :any:`mge::context_object`, i.e. it exists in the scope
of a render context.

Types and Definitions
=====================

Buffers are defined by buffer type.

.. doxygenenum:: mge::buffer_type

.. doxygenenum:: mge::data_type

.. doxygenfunction:: mge::data_type_size

Hardware Buffer
===============

Base of all buffer types is the hardware buffer.

.. doxygenclass:: mge::hardware_buffer
    :members:

Vertex Buffer
=============

Vertex buffers are used to provide vertices, i.e. data that is processed processed
per vertex. This may be a simple 2D or 3D position, or a combination of position,
texture coordinate, and normal vector data.

A vertex format defines an element of data in a vertex buffer.

.. doxygenclass:: mge::vertex_format
    :members:

To define the data layout in a vertex buffer, a vertex layout is defined, which
is a list of vertex formats:

.. doxygenclass:: mge::vertex_layout
    :members:

Vertex layouts are used to defined the structure of a vertex buffer.

.. doxygenclass:: mge::vertex_buffer
    :members:


Index Buffer
============

Index buffers are used to provide indices into vertex buffers.

.. doxygenclass:: mge::index_buffer
    :members:

Constant Buffer
===============