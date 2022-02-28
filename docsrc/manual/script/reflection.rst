*****************
Reflection Types
*****************

Reflection allows a program to introspect upon itself, and manipulate
internal program properties. It allows also the exposure of reflected
objects to scripting languages.

All types used in reflection support are in the namespace ``mge::script``.

Module
======

A module encapsulates a namespace.

.. doxygenclass:: mge::script::module
    :members:

Type
====

A type captures the properties of a type. It can be a simple type or a complex
class type.

.. doxygenclass:: mge::script::type
    :members: