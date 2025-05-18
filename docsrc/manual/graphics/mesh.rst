****
Mesh
****

A mesh provides an interface to vertex data and defined faces. Note that a mesh
only encapsulates vertex data and the indices of the faces, but does not 
include textures, bones, or any hierarchy information. 

A 'memory_mesh' is a mesh that owns its own memory.

Types and Definitions
=====================

.. doxygenclass:: mge::mesh
    :members:

..doxygenclass:: mge::memory_mesh
    :members: