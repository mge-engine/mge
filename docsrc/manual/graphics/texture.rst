*******
Texture
*******

A texture is an image or a collection of data that is applied to a 3D model
or a 2D surface, or used in a shader program. Textures are used to enhance the
visual appearance of objects and help to simulate various materials, for example
wood or stone.

Textures are typically initialized from image files, but can be also generated
by rendering to a texture.

When textures are applied to a model or surface, texture coordinates are used
to map the texture onto the surface. This process is known as texture mapping.

A texture is a :any:`mge::context_object`, i.e. it exists in the scope
of a render context.

Types and Definitions
=====================

.. doxygenenum:: mge::texture_type

.. doxygenclass:: mge::texture
    :members:

