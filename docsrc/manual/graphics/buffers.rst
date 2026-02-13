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

.. doxygenenum:: mge::topology

Uniform data types specify data types for shader uniform variables.

.. doxygenenum:: mge::uniform_data_type

.. doxygenfunction:: mge::uniform_data_type_size

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

.. doxygenenum:: mge::attribute_semantic

To define the data layout in a vertex buffer, a vertex layout is defined, which
is a list of vertex formats, and an added attribute semantic.

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

Constant buffers are used to provide constant data to shader programs.

Uniform Block
=============

Uniform blocks provide a structured way to pass uniform data to shaders with automatic
std140 layout management. Uniform blocks are created from a program's uniform buffer 
metadata and manage CPU-side data that is synchronized to GPU buffers.

A uniform block represents a named group of uniform variables in a shader program. The
layout follows the std140 standard to ensure consistent packing across different platforms.

.. doxygenclass:: mge::uniform_block
    :members:

Key Features
------------

* **Automatic Layout**: Uniform blocks automatically compute std140-compliant layout from program metadata
* **Type Safety**: Member access is type-checked via the ``set<T>()`` template method
* **Version Tracking**: Each update increments a version counter for efficient GPU synchronization
* **Cross-Platform**: Works consistently across OpenGL, DirectX 11, DirectX 12, and Vulkan

Usage Example
-------------

The ``rotating_triangle`` sample demonstrates uniform block usage::

    // After program linking, get uniform buffer metadata
    const auto& uniform_buffers = program->uniform_buffers();
    
    // Create uniform block from metadata
    auto block = std::make_unique<uniform_block>(uniform_buffers[0]);
    
    // Update uniform values
    float angle = 45.0f;
    block->set("angle", angle);
    
    // Bind block to command buffer for drawing
    command_buffer.bind_uniform_block(block.get());
    command_buffer.draw(program, vertices, indices);

The uniform block is automatically uploaded to the GPU when version changes are detected.

Std140 Layout
-------------

MGE uses std140 layout rules for uniform blocks:

.. doxygenfunction:: mge::std140_base_alignment
.. doxygenfunction:: mge::std140_type_size
.. doxygenfunction:: mge::std140_array_stride
.. doxygenfunction:: mge::std140_align
