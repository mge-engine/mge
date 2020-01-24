.. _mgecore_utility:

*********
Utilities
*********

Type Utilities
==============

File :file:`mge/core/types.hpp` provides some utilities for type declarations.

.. doxygendefine:: MGE_DECLARE_REF
    :project: mge

.. doxygentypedef:: mge::noncopyable
    :project: mge

Array Size
==========

Determining the number of elements in an array is usually done with some
``sizeof`` trickery, which is easy to get wrong, or which may not be updated
when an array type changes. Include :file:`mge/core/array_size.hpp` for a
helper function:

.. doxygenfunction:: mge::array_size
    :project: mge

Bit Operations
==============

The file :file:`mge/core/bit.hpp` contains a number of helper functions
for commonly used bit operations and abstractions for compiler intrinsics.

.. doxygenfunction:: mge::popcount(uint8_t)
    :project: mge
.. doxygenfunction:: mge::popcount(uint16_t)
    :project: mge
.. doxygenfunction:: mge::popcount(uint32_t)
    :project: mge
.. doxygenfunction:: mge::popcount(uint64_t)
    :project: mge

Boolean Value
=============

The :cpp:any:`mge::bool_value` function defined in
:file:`mge/core/bool_value.hpp` provides a convenient way to determine
the value of a boolean setting or option. These functions accept "true", "yes", "on" and any non-zero number
as ``true`` and produce ``false`` for anything else.

.. doxygenfunction:: mge::bool_value(const char *)
    :project: mge
.. doxygenfunction:: mge::bool_value(const std::string&)
    :project: mge
.. doxygenfunction:: mge::bool_value(std::string_view)
    :project: mge

