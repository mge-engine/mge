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

Checked Cast
============

Numeric casts in C++ have the danger of over- or underflow, especially when
casting to a smaller integer type or from signed to unsigned types. File
:file:`mge/core/checked_cast.hpp` provides the cpp:any:`mge::checked_cast`
helper function:

.. doxygenfunction:: mge::checked_cast
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

.. doxygenfunction:: mge::popcount(uint8_t x)
    :project: mge
.. doxygenfunction:: mge::popcount(uint16_t x)
    :project: mge
.. doxygenfunction:: mge::popcount(uint32_t x)
    :project: mge
.. doxygenfunction:: mge::popcount(uint64_t x)
    :project: mge

Boolean Value
=============

The :cpp:any:`mge::bool_value` function defined in
:file:`mge/core/bool_value.hpp` provides a convenient way to determine
the value of a boolean setting or option. These functions accept "true", "yes", "on" and any non-zero number
as ``true`` and produce ``false`` for anything else.

.. doxygenfunction:: mge::bool_value(const char *str) noexcept
    :project: mge
.. doxygenfunction:: mge::bool_value(const std::string &s) noexcept
    :project: mge
.. doxygenfunction:: mge::bool_value(std::string_view s) noexcept
    :project: mge

Buffer
======

The type :any:`mge::buffer` is defined to be a ``std::vector<uint8_t>``
as buffers of binary data are ubiquitously used througout the
engine.

.. doxygentypedef:: mge::buffer
    :project: mge

Line Printer
============

The function :any:`mge:line_printer` defined in
:file:`mge/core/streamutils.hpp` makes it easy to print a line of designated
length.

.. doxygenfunction:: mge::line
    :project: mge

Clear Function
==============

The function :any:`mge:clear_function` defined in
:file:`mge/core/clear_function.hpp` resets a ``std::function``
object.

.. doxygenfunction:: mge::clear_function
    :project: mge

Contains
========

The function :any:`mge::contains` defined in
:file:`mge/core/contains.hpp` determines whether a value is contained
in a container. While ``std::map`` defines a ``find`` member function,
e.g. vector classes don't implement one, leaving a find tedious to write.

.. doxygenfunction:: mge::contains(const Container &c, const Element &e)
    :project: mge

Copy Structure
==============

The function :any:`mge::copy_struct` is a helper the helps copying
structures which just contain data, and no object type requiring
copy constructors.

.. doxygenfunction:: mge::copy_struct(T &target, const T &source)
    :project: mge

.. doxygenfunction:: mge::copy_struct(T *target, const T &source)
    :project: mge

Format String
=============

.. doxygenfunction:: mge::format_string
    :project: mge

Stream Values
=============

.. doxygenfunction:: mge::stream_values(std::ostream &os, T value, Args... args)
    :project: mge

Enum Set
========

Given an enum type ``E`` an enum set is a set where each value of
the enum can be set and cleared. For efficiency, the enum type
should start with 0 and have contiguous values.

.. doxygenclass:: mge::enum_set
    :project: mge
    :members:

Function Map
============

A helper class to map a function to a key (an ``unsigned int``). This mechanism
is internally used for listener registration and alike scenarios.

.. doxygenclass:: mge::function_map
    :project: mge
    :members:

Gist
====

A *gist* is a small information on something, i.e. not a complete dump but
a in information with less detail. E.g. for vector, where a dump would be
printing all members, a gist is just printing how many elements the
vector has.

A gist is used as follows:

.. code-block:: c++

    MyComplicatedObject o;
    std::cout << "Gist of object: " << gist(o) << std::endl;

To implement a gist, one has two options - gist method or gist operator.

Gist method
...........

A method with the signature ``void gist(std::ostream& os) const`` is picked
up automatically by the gist global method.

Gist type operator
..................

The helper type :any:`mge::gist_type` exist to provide a value wrapper for
gist printing.

.. doxygenstruct:: mge::gist_type
    :project: mge
    :members:

A helper macro exist for gist implementation:

.. doxygendefine:: MGE_GIST_OUTPUT
    :project: mge

Property Handling
=================

Properties
..........

A key-value map of strings is sometimes useful, and the
:any:`mge::properties` can be used to handle such a list
efficiently:

.. doxygenclass:: mge::properties
    :project: mge
    :members:

Property Object
...............

Sometimes an object is needed that holds proprties in addition
to other functionality. :any:`mge::property_object` serves this
purpose:

.. doxygenclass:: mge::property_object
    :project: mge
    :members:

Sequence
========

..doxygenclass:: mge::sequence
    :project: mge
    :members:

