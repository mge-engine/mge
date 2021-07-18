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

.. doygenfunction:: mge::data_type_size

Hardware Buffer
===============

Base of all buffer types is the hardware buffer.

.. doxygenclass:: mge::hardware_buffer
    :members:

Vertex Buffer
=============

Index Buffer
============

Index buffers are used to provide indices into vertex buffers.

.. doxygenclass:: mge::index_buffer
    :members:

Constant Buffer
===============